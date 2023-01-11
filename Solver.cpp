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



Solution Solver::Heuristique1() {
    //Quantite de surface disponible a un instant t


    Solution result;

    Culture  best_crop;
    float best_reward;
    float best_quantity;
    int best_start;

    int latest_end;
    int early_start;

    float alpha;
    float beta;
    float max_quantity_possible;
    float max_reward_possible;

    result = Solution(instance,worst_scenario);

    auto iter_on_water = result.waterAtT.begin() ;
    auto iter_on_land = result.landAtT.begin();

    best_reward = -1;

    while( best_reward != 0){
        best_reward = -1;
        for(Culture c : instance.cultures){
            early_start = c.depart_tot;
            latest_end = c.depart_tard + c.duree_pousse;

            //valeur min d'apport en eau sur la periode de pousse possible de la plante
            alpha = *std::min_element(iter_on_water + early_start , iter_on_water + latest_end );

            //valeur min de terre disponible sur la periode de pousse possible de la plante
            beta = *std::min_element(iter_on_land + early_start , iter_on_land + latest_end );

            /*
             * On calcule la quantite maximale obtenable
             * min apport_en_eau / besoin_en_eau_pour_tonne donne la quantite max par rapport a l'eau disponible
             * min terre_dispo / besoin_en_terre_pour_tonne donne la quantite max par rapport a la terre disponible
             */
            max_quantity_possible = std::min(alpha / c.besoin_eau, beta / c.hectars_pour_tonne);

            // calcule le score max d'une culture
            max_reward_possible = max_quantity_possible * c.rendement;

            //on garde la culture qui rapporte le plus
            if(max_reward_possible > best_reward){
                best_crop = c;
                best_reward = max_reward_possible;
                best_quantity = max_quantity_possible;
                best_start = early_start;
            }
        }
        if (best_reward != 0){
            result.AllocateCrop(best_crop,best_quantity, best_start);
        }
    }



    return result;
}
