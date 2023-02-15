//
// Created by Bastien Camembert on 11/01/2023.
//

#include "Solver.h"

#include <utility>

Solver::Solver(Instance i): instance(std::move(i)), worstScenario(Scenario()){
    MakeWorstScenario();
}
void Solver::MinCumulScenario(){
    int range = instance.nbWeeks;
    int nbScenarios = instance.nbScenarios;
    worstScenario.apport_hebdomadaire = std::vector<float>(range);
    std::vector<Scenario> scenarios = instance.scenarios;


    int week; // week iterator
    int sIndex; //scenario iterator
    float minCumul;
    float currentCumul;
    float initValue;
    float firstSupply;
    float cumulValue;

    Scenario currentScenario;

    std::vector<float> cumuls = std::vector<float>(nbScenarios);

    //Building worst weekly delivery

    // Worst initial delivery
    minCumul = 999999;
    worstScenario.apport_initial = 99999;
    for(sIndex = 0; sIndex < nbScenarios; sIndex++){
        currentScenario = scenarios[sIndex];
        initValue = currentScenario.apport_initial;
        firstSupply = currentScenario.apport_hebdomadaire[0];
        cumuls[sIndex] = firstSupply + initValue;
        if(firstSupply<minCumul)
            minCumul = firstSupply;
        if(initValue < worstScenario.apport_initial)
            worstScenario.apport_initial = initValue;

    }
    worstScenario.apport_hebdomadaire[0] = minCumul;
    currentCumul = minCumul + initValue;

    for(week = 1; week < range; week++){
        minCumul = 999999;
        for(sIndex = 0; sIndex < nbScenarios; sIndex++){
            currentScenario = scenarios[sIndex];
            cumulValue = cumuls[sIndex] + currentScenario.apport_hebdomadaire[week];
            cumuls[sIndex] = cumulValue;
            if(cumulValue<minCumul)
                minCumul = cumulValue;
        }
        worstScenario.apport_hebdomadaire[week] = minCumul - currentCumul;
        currentCumul = minCumul;

    }
}

/**
 * Create an artificial scenario which is the worst possible for the instance
 * @return result in worst scenario reference
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
 * to find the best crop's configuration
 * @param solution
 * @param culture
 * @return
 */
std::pair<int,float> Solver::FindBestConfig(Solution &solution, Culture &culture){

    int week; // week iterator
    float alpha,alphaTemp,beta; //minimum water and land delivery on the growth duration window
    float maxQuantityPossible,maxRewardPossible; //maximum possible to stay feasible

    float gasEmiRemaining =solution.instance.maxGreenhouseGases - solution.greenhouseGasEmission;

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

    bestReward = 0;
    bestQuantity = 0;
    bestDate = earlyStart;

    for(week=earlyStart; week <= latestStart ; week++){
        //minimum water delivery on the growth duration window
        alpha = 9999999;
        for (int t = 0; t<growthDuration; t++){
            alphaTemp = *(iterOnWater + week +t)/((t+1)*waterNeeds);
            if(alphaTemp<alpha)
                alpha = alphaTemp;


        }


        //alpha = *std::min_element(iterOnWater + week , iterOnWater + week + growthDuration );
        //minimum land delivery on the growth duration window
        beta = *std::min_element(iterOnLand + week , iterOnLand + week + growthDuration );

        //Compute the maximum quantity possible
        maxQuantityPossible = std::min(alpha , beta / landNeeds);

        maxQuantityPossible = std::min(maxQuantityPossible, gasEmiRemaining/cropEmissions);
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
 *
 * @return
 */
Solution Solver::Heuristique1() {
    MinCumulScenario();
    Solution result;

    //best crop infos
    Culture  bestCrop;
    float bestReward,bestQuantity;
    int bestStart;

    //crop's config
    int start;
    float maxQuantityPossible;
    float maxRewardPossible;
    std::pair<int,float> cropConfig;

    result = Solution(instance, worstScenario);
    //result = Solution(instance,instance.scenarios[0]);
    //score
    bestReward = -1;

    while(bestReward != 0){
        bestReward = -1;
        for(Culture c : instance.crops){

            cropConfig = FindBestConfig(result, c);
            start = cropConfig.first;
            maxQuantityPossible = cropConfig.second;
            maxRewardPossible = maxQuantityPossible * c.rendement;

            //memorize the best crop
            if(maxRewardPossible > bestReward){
                bestCrop = c;
                bestReward = maxRewardPossible;
                bestQuantity = maxQuantityPossible;
                bestStart = start;
            }

        }
        //apply the best crop
        if (bestReward != 0){
            result.AllocateCrop(bestCrop, bestQuantity, bestStart,true);
            bestReward = -1;
            bestQuantity = 0;
            bestStart = 0;
        }
    }
    result.end();
    return result;
}
