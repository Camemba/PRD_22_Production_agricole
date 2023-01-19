//
// Created by Bastien Camembert  on 03/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_SCENARIO_H
#define PRD_22_PRODUCTION_AGRICOLE_SCENARIO_H
#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


class Scenario {
public:
    Scenario();


public:
    float apport_initial; //apport en eau au depart
    std::vector<float> apport_hebdomadaire; //apport en eau pour chaque semaine

    /*
* Macro pour "JSONiser" la classe
* TYPE_INTRUSIVE permet d'acceder e des attributs prives
* Chaque sous-classe utilisee doit etre JSONisable elle aussi
*/
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Scenario, apport_initial, apport_hebdomadaire)
};
std::ostream& operator<<(std::ostream& os, const Scenario& s);

#endif //PRD_22_PRODUCTION_AGRICOLE_SCENARIO_H
