//
// Created by camem on 11/01/2023.
//

#include "Solver.h"

Solver::Solver(Instance i): instance(i),worst_scenario(Scenario()){
    makeWorstScenario();
}

/**
 * Create an artificial scenario which is the worst possible for the instance
 * @return worst scenario reference
 */
void Solver::makeWorstScenario() {
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
    return Solution();
}
