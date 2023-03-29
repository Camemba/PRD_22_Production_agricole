//
// Created by Bastien Camembert on 11/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_SOLVER_H
#define PRD_22_PRODUCTION_AGRICOLE_SOLVER_H


#include "models/Solution.h"

class Solver {
public:
    const Instance instance;
    Scenario worstScenario;
public:
    explicit Solver(Instance i);
    Solution Heuristique1();
private:
    void MakeWorstScenario();
    static std::pair<int,float>  FindBestConfig(const Solution &solution, const Culture &culture);

    void MinCumulScenario();

    static void Repair(Solution &solution);
    static void SolveKnapsack(Solution &solution);


    void SolveLandProblem(Solution &solution, int conflictWeek);
};

#endif //PRD_22_PRODUCTION_AGRICOLE_SOLVER_H
