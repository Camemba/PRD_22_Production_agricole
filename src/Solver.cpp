//
// Created by Bastien Camembert on 11/01/2023.
//

#include "Solver.h"
#include <utility>

Solver::Solver(Instance i): instance(std::move(i)), worstScenario(Scenario()){
    MakeWorstScenario();
}
/**
 * Create an artificial scenario based on the minimal cumulating water supply  of all scenario
 * supply[T] = minCumul[T] - minCumul[T-1]
 * @return result in worstScenario reference
 */
void Solver::MinCumulScenario(){

    //Datas needed
    int range = instance.nbWeeks;
    int nbScenarios = instance.nbScenarios;
    std::vector<Scenario> scenarios = instance.scenarios;

    worstScenario.apport_hebdomadaire = std::vector<float>(range); //init the worst scenario supply list shape
    std::vector<float> cumuls = std::vector<float>(nbScenarios);// cumulating value of each scenario

    //Temporary values
    int week; // week iterator
    int sIndex; //scenario iterator
    float minCumul; //temporary min cumulating value
    float currentCumul; // current cumulating value of the scenario being built
    float initValue; //initialisation value of the studied scenario
    float firstSupply;//first supply value of the studied scenario
    float cumulValue;//cumulating value of the studied scenario
    Scenario currentScenario; // studied scenario


    minCumul = 999999;
    worstScenario.apport_initial = 99999;
    //Initialisation of the worst scenario
    //Find the min(init + first) in all scenarios
    for(sIndex = 0; sIndex < nbScenarios; sIndex++){
        currentScenario = scenarios[sIndex];
        initValue = currentScenario.apport_initial;
        firstSupply = currentScenario.apport_hebdomadaire[0];
        cumuls[sIndex] = firstSupply + initValue;

        //Save min values
        if(firstSupply<minCumul)
            minCumul = firstSupply;
        if(initValue < worstScenario.apport_initial)
            worstScenario.apport_initial = initValue;

    }
    worstScenario.apport_hebdomadaire[0] = minCumul;// at this state minCumul is just the min first supply
    currentCumul = minCumul + initValue; // init the first cumulating value of the being built scenario

    for(week = 1; week < range; week++){
        minCumul = 999999;
        for(sIndex = 0; sIndex < nbScenarios; sIndex++){
            currentScenario = scenarios[sIndex];
            cumulValue = cumuls[sIndex] + currentScenario.apport_hebdomadaire[week];
            cumuls[sIndex] = cumulValue;// Cumul[t] = cumul[t-1] + supply[t]

            //save the min cumul[t] for all scenario
            if(cumulValue<minCumul)
                minCumul = cumulValue;
        }

        worstScenario.apport_hebdomadaire[week] = minCumul - currentCumul;//Actualise the being build scenario
        currentCumul = minCumul;//actualise the cumul value

    }
}

/**
 * Create an artificial scenario based on the minimal water supply of all scenario
 * @return result in worstScenario reference
 */
void Solver::MakeWorstScenario() {
    int range = instance.nbWeeks;
    worstScenario.apport_hebdomadaire = std::vector<float>(range);
    worstScenario.apport_initial = 99999;

    int week; // week iterator
    float worstWeekValue; //min week value for all scenarios
    float weekValue;  //temporary value for weekly delivery
    float initValue; //temporary value for initial delivery

    //Building worst weekly delivery
    for(week = 0; week < range; week++){
        worstWeekValue = 99999;

        //worst week for all scenarios
        for(Scenario s : instance.scenarios){
            weekValue = s.apport_hebdomadaire[week];
            if(weekValue < worstWeekValue)
                worstWeekValue = weekValue;
        }
        //update our worst scenario
        worstScenario.apport_hebdomadaire[week] = worstWeekValue;
    }
    // Worst initial delivery
    for(const Scenario& s : instance.scenarios){
        initValue = s.apport_initial;
        if(initValue < worstScenario.apport_initial)
            worstScenario.apport_initial = initValue;
    }
}


/**
 * Move a growth duration window between [early start and latest start + growth duration]
 * to find the best crop's plantation date to maximise the reward
 * @param solution current resolution state of the problem
 * @param culture crop to study
 * @return a pair of (best date, best quantity)
 */
std::pair<int,float> Solver::FindBestConfig(const Solution &solution, const Culture &culture){

    //crop's infos
    int earlyStart = culture.depart_tot;
    int latestStart = culture.depart_tard;
    int growthDuration = culture.duree_pousse;
    float waterNeeds = culture.besoin_eau;
    float landNeeds = culture.hectars_pour_tonne;
    float cropReward = culture.rendement;
    float cropEmissions = culture.emission;

    //outputs
    int bestDate ;
    float bestReward,bestQuantity;
    std::pair<int,float> result;

    //water and land availability
    auto iterOnWater = solution.waterAtT.begin() ;
    auto iterOnLand = solution.landAtT.begin();

    int week; // week iterator
    float upperBoundWater; // maximum quantity possible based on the min water availability
    float upperBoundLand; // maximum quantity possible based on the min land availability
    float tempMinLand,tempMinWater;
    float maxQuantityPossible,maxRewardPossible; //maximum possible to stay feasible

    float gasEmiRemaining =solution.instance.maxGreenhouseGases - solution.greenhouseGasEmission;

    bestReward = 0;
    bestQuantity = 0;
    bestDate = latestStart;

    for(week=earlyStart; week <= latestStart ; week++){

        //Compute the min upperBound
        upperBoundWater = 9999999;
        upperBoundLand = 999999;
        for (int t = 0; t<growthDuration; t++){
            tempMinWater = *(iterOnWater + week +t)/((float)(t+1)*waterNeeds);
            tempMinLand = *(iterOnLand + week + t)/ landNeeds;
            if(0 < tempMinWater < upperBoundWater)
                upperBoundWater = tempMinWater;

            if(0 < tempMinLand < upperBoundLand)
                upperBoundLand = tempMinWater;
        }

        //Compute the maximum quantity possible
        maxQuantityPossible = std::min(upperBoundWater , upperBoundLand);
        maxQuantityPossible = std::min(maxQuantityPossible, gasEmiRemaining/cropEmissions);

        //avoid micro quantity
        if(maxQuantityPossible < 0.001)
            maxQuantityPossible = 0;

        maxRewardPossible = maxQuantityPossible * cropReward; //crop score

        // memorize config if better
        if(maxRewardPossible > bestReward){
            bestReward = maxRewardPossible;
            bestQuantity = maxQuantityPossible;
            bestDate = week;
        }
    }
    result = std::pair(bestDate,bestQuantity);
    return result;
}

/**
 * Build a feasible solution on worst scenario
 * there are different ways to build the worst scenario which influence the solution quality
 * and the feasibility on the global problem
 * @return a complete solution object
 */
Solution Solver::Heuristique1() {
    MinCumulScenario();
    std::vector<Culture>crops;
    std::map<Culture,bool>chosenCrops;
    Culture crop;


    //best crop infos
    Culture bestCrop;
    float bestReward,bestQuantity;
    int bestStart;



    //crop's config
    int start;
    float maxQuantityPossible;
    float maxRewardPossible;
    std::pair<int,float> cropConfig;

    Solution result = Solution(instance, worstScenario);
    bestReward = -1;
    // init crop list


    while(bestReward != 0 ){
        bestReward = 0;
        for(const auto & iterOnCrop : instance.crops){

            crop = iterOnCrop;
            cropConfig = FindBestConfig(result, crop);
            start = cropConfig.first;
            maxQuantityPossible = cropConfig.second;
            maxRewardPossible = maxQuantityPossible * crop.rendement;

            //memorize the best crop
            if(maxRewardPossible > bestReward && chosenCrops[crop]!=1){
                bestCrop = iterOnCrop;
                bestReward = maxRewardPossible;
                bestQuantity = maxQuantityPossible;
                bestStart = start;
            }



        }
        //apply the best crop
        if (bestReward != 0){
            result.AllocateCrop(bestCrop, bestQuantity, bestStart);
            chosenCrops[bestCrop]=true;
            bestReward = -1;
            bestQuantity = 0;
            bestStart = 0;

        }
    }
    result.end();
    return result;
}
/**
 * Solve the real quantity knapsack problem
 * @param solution
 * @param emission decide if W,wi are crop emission or land needs
 */
void Solver::SolveKnapsack(Solution& solution){
    auto items =std::vector<std::pair<float, const Culture *>>() ; //list (weight/reward, item)
    auto decisionMap = solution.affectedQuantity;//decision of the solution in parameter
    Solution newSolution = Solution(solution);
    //knapsack problem variables
    float W,wi,ri;

    //new decision variable
    float quantity;
    int date;
    const Culture* crop;

    //crop list variable
    int itemIndex;
    int size;


    //data initialisation
    W = solution.instance.maxGreenhouseGases;

    for(auto & iterCultureMap : decisionMap){
        crop = &(iterCultureMap.first);
        wi = crop->emission;
        ri = crop->rendement;
        items.emplace_back(wi/ri,crop);
    }
    //the smaller the first value is, the better the item is.
    std::sort(items.begin(), items.end());
    itemIndex = 0;
    size = (int)items.size();
    while( W>0 || itemIndex<size){
        crop = items[itemIndex].second;
        wi = crop->emission;
        quantity = W/wi;
        date = decisionMap[*crop].begin()->first; // use the first plantation date of a crop
        newSolution.AllocateCrop(*crop,quantity,date);
    }
    solution = newSolution;
    
}
void Solver::SolveLandProblem(Solution& solution, int conflictWeek){
    auto items =std::vector<std::pair<int, const Culture *>>() ; //list (date, item)
    auto decisionMap = solution.affectedQuantity;//decision of the solution in parameter
    Solution newSolution = Solution(solution);

    for(auto & iterCultureMap : decisionMap){
        
    }
}


/**
 * Repair the solution if unfeasible, do nothing otherwise
 * @param solution
 */
void Solver::Repair(Solution& solution){
    try{
        solution.Verify(true);
    }catch (ConstraintViolationException& cve){
        Constraint type = cve.type();

        switch (type) {
            case Constraint::GGE:
                //knapsack problem where emission are weights
                SolveKnapsack(solution);
                break;
            case Constraint::Water:
                break;
            case Constraint::Land:

                break;
            default:
                std::cout<<"Exception type not recognize in repair function : "<<cve.what()<<std::endl;
        }
        Repair(solution);//Recall the function to verify if solution is
    }
}
