//
// Created by camem on 03/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_SCENARIO_H
#define PRD_22_PRODUCTION_AGRICOLE_SCENARIO_H
#include <vector>

class Scenario {
public:
    Scenario();

public:
    float apport_initial; //apport en eau au départ
    std::vector<float> apport_hebdomadaire; //apport en eau pour chaque semaine
};


#endif //PRD_22_PRODUCTION_AGRICOLE_SCENARIO_H
