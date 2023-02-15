//
// Created by Bastien Camembert on 11/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_SOLVER_H
#define PRD_22_PRODUCTION_AGRICOLE_SOLVER_H


#include "Solution.h"

class Solver {
public:
    Instance instance;
    Scenario worstScenario;
public:
    explicit Solver(Instance i);
    Solution Heuristique1();
private:
    void MakeWorstScenario();
    static std::pair<int,float>  FindBestConfig(Solution &solution, Culture &culture);

    void MinCumulScenario();
};

#endif //PRD_22_PRODUCTION_AGRICOLE_SOLVER_H
