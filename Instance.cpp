//
// Created by Bastien Camembert on 03/01/2023.
//

#include "Instance.h"

Instance::Instance() : id(), nbCrops(), nbWeeks(), nbScenarios(),
                       amountLands(), maxGreenhouseGases(), crops({}), scenarios({}) {}

 void Instance::BuildFromJson(const json& js, Instance& instance)
{
    js.at("id").get_to(instance.id);
    js.at("nb_jobs").get_to(instance.nbCrops);
    js.at("horizon").get_to(instance.nbWeeks);
    js.at("nb_scenarios").get_to(instance.nbScenarios);
    js.at("nb_hectars").get_to(instance.amountLands);
    js.at("GESMAX").get_to(instance.maxGreenhouseGases);

    auto data_c = js.at("cultures");
    for (const json& json_culture : data_c){
        Culture culture = Culture();
        nlohmann::from_json(json_culture,culture);
        instance.crops.push_back(culture);
    }
    auto data_s = js.at("scenarios");
    for (const json& json_scenario : data_s){
        Scenario scenario = Scenario();
        nlohmann::from_json(json_scenario,scenario);
        instance.scenarios.push_back(scenario);
    }


}
std::ostream& operator<<(std::ostream& os, const Instance& instance)
{   os <<std::endl;
    os << "id : " << instance.id << std::endl;
    os << "nbCrops : " << instance.nbCrops << std::endl;
    os << "nbWeeks : " << instance.nbWeeks << std::endl;
    os << "nbScenarios : " << instance.nbScenarios << std::endl;
    os << "amountLands : " << instance.amountLands << std::endl;
    os << "maxGreenhouseGases : " << instance.maxGreenhouseGases << std::endl;
    os << "crops : "<<std::endl;
    for (const Culture& c : instance.crops)
        os <<c <<std::endl;
    os << "scenarios : "<<std::endl;
    for (const Scenario& s : instance.scenarios)
        os << s <<std::endl;
    return os;
}