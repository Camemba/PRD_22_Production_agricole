//
// Created by camem on 11/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_SOLVER_H
#define PRD_22_PRODUCTION_AGRICOLE_SOLVER_H


#include "Instance.h"
#include "Solution.h"

class Solver {
public:
    Instance instance;
    Scenario worst_scenario;
public:
    Solver(Instance i);
    Solution Heuristique1();
private:
    void MakeWorstScenario();

    bool Verification(Solution solution);

    std::pair<int,float>  FindBestConfig(Solution &solution, Culture &culture);
};


#endif //PRD_22_PRODUCTION_AGRICOLE_SOLVER_H
