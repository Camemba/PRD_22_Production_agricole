//
// Created by camem on 11/01/2023.
//

#include "Solver.h"

Solver::Solver(Instance i): instance(i),worst_scenario(Scenario()){
    MakeWorstScenario();
}

/**
 * Create an artificial scenario which is the worst possible for the instance
 * @return worst scenario reference
 */
void Solver::MakeWorstScenario() {
    int range = instance.horizon;
    worst_scenario.apport_hebdomadaire = std::vector<float>(range);
    worst_scenario.apport_initial = 99999;

    int week;
    float worst_week_value;
    float week_value;
    float init_value;

    //Building du pire apport_hebdo
    for(week = 0; week < range; week++){
        worst_week_value = 99999;

        //la pire semaine de tous les scenarios
        for(Scenario s : instance.scenarios){
            week_value = s.apport_hebdomadaire[week];
            if(week_value < worst_week_value)
                worst_week_value = week_value;
        }
        //mise a jour de notre pire scenario
        worst_scenario.apport_hebdomadaire[week] = worst_week_value;
    }
    // le pire apport initial
    for(Scenario s : instance.scenarios){
        init_value = s.apport_initial;
        if(init_value < worst_scenario.apport_initial)
            worst_scenario.apport_initial = init_value;
    }
}


bool Solver::Verification(Solution solution){
    bool solutionIsValid = true;

    auto solutionMap = solution.affectedQuantity;
    // Pour chaque culture
    for(auto mapIterator = solutionMap.begin();mapIterator !=solutionMap.end();++mapIterator){
        // Pour chaque plantation
        for(auto listIterator = mapIterator->second.begin();listIterator!=mapIterator->second.end();++listIterator){

        }
    }
    return solutionIsValid;

}
/**
 * Move a growth duration window between [early start and latest start + growth duration]
 * to find the best crop's configuration
 * @param solution
 * @param culture
 * @return
 */
std::pair<int,float> Solver::FindBestConfig(Solution &solution, Culture &culture){

    int week;
    float alpha,beta;
    float maxQuantityPossible,maxRewardPossible;

    int earlyStart = culture.depart_tot;
    int latestStart = culture.depart_tard;
    int growthDuration = culture.duree_pousse;
    float waterNeeds = culture.besoin_eau;
    float landNeeds = culture.hectars_pour_tonne;
    float cropReward = culture.rendement;

    int bestDate ;
    float bestReward,bestQuantity;

    auto iterOnWater = solution.waterAtT.begin()+earlyStart ;
    auto iterOnLand = solution.landAtT.begin()+earlyStart;

    bestReward = 0;
    bestQuantity = 0;
    bestDate = earlyStart;

    /*
     * Move a growth duration window between [early start and latest start + growth duration]
     * to find the best crop's configuration
     */
    for(week=0; week < latestStart - earlyStart; week++){
        //minimum water delivery on the growth duration window
        alpha = *std::min_element(iterOnWater + week , iterOnWater + week + growthDuration );
        //minimum land delivery on the growth duration window
        beta = *std::min_element(iterOnLand + week , iterOnLand + week + growthDuration );

        /*
         * On calcule la quantite maximale obtenable
         * min apport_en_eau / besoin_en_eau_pour_tonne donne la quantite max par rapport a l'eau disponible
         * min terre_dispo / besoin_en_terre_pour_tonne donne la quantite max par rapport a la terre disponible
         */
        maxQuantityPossible = std::min(alpha / waterNeeds, beta / landNeeds);
        maxRewardPossible = maxQuantityPossible * cropReward; //crop score

        if(maxRewardPossible > bestReward){
            bestReward = maxRewardPossible;
            bestQuantity = maxQuantityPossible;
            bestDate = earlyStart+week;
        }
    }

    return std::pair<int,float>(bestDate,bestQuantity);
}


Solution Solver::Heuristique1() {
    //Quantite de surface disponible a un instant t


    Solution result;

    Culture  best_crop;
    float best_reward;
    float best_quantity;
    int best_start;

    int latest_end;
    int start;

    float alpha;
    float beta;
    float max_quantity_possible;
    float max_reward_possible;

    result = Solution(instance,worst_scenario);
    //result = Solution(instance,instance.scenarios[0]);

    auto iter_on_water = result.waterAtT.begin() ;
    auto iter_on_land = result.landAtT.begin();
    std::pair<int,float> bestConfig;
    best_reward = -1;

    while( best_reward != 0){
        best_reward = -1;
        for(Culture c : instance.cultures){

            bestConfig = FindBestConfig(result,c);
            start = bestConfig.first;
            max_quantity_possible = bestConfig.second;
            std::cout<<max_quantity_possible<<std::endl;
            // calcule le score max d'une culture
            max_reward_possible = max_quantity_possible * c.rendement;

            //on garde la culture qui rapporte le plus
            if(max_reward_possible > best_reward){
                best_crop = c;
                best_reward = max_reward_possible;
                best_quantity = max_quantity_possible;
                best_start = start;

                std::cout<<"best : "<<best_reward<<" "<<best_quantity<<std::endl;
            }
            std::cout<<" "<<std::endl;
        }
        if (best_reward != 0){
            result.AllocateCrop(best_crop,best_quantity, best_start);
            best_reward = 0;
            best_quantity = 0;
            best_start = 0;
        }
    }



    return result;
}
