//
// Created by Bastien Camembert on 03/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_INSTANCE_H
#define PRD_22_PRODUCTION_AGRICOLE_INSTANCE_H

#include <iostream>
#include <fstream>

#include "Culture.h"
#include "Scenario.h"
using json = nlohmann::json;

class Instance {
public:
    int id;
    int nbCrops; //number of different crop
    int nbWeeks; //total number of weeks
    int nbScenarios; // number of scenario
    int amountLands; //total amount of lands
    float maxGreenhouseGases; //greenhouse gases
    std::vector<Culture> crops; //available crops
    std::vector<Scenario> scenarios; //scenarios list

    //Enable Instance class to be converted to JSON
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Instance, id, nbCrops, nbWeeks, nbScenarios, amountLands, maxGreenhouseGases, crops)
public:
    Instance();
    static void BuildFromJson(const json& js, Instance& instance);

};
std::ostream& operator<<(std::ostream& os, const Instance& instance);

#endif //PRD_22_PRODUCTION_AGRICOLE_INSTANCE_H
