//
// Created by camem on 03/01/2023.
//

#include "Instance.h"

Instance::Instance() :id(),nb_jobs(),horizon(),nb_scenarios(),
        nb_hectars(), GESMAX(), cultures({}), scenarios({}) {}

 void Instance::from_json(const json& js, Instance& instance)
{
    js.at("id").get_to(instance.id);
    js.at("nb_jobs").get_to(instance.nb_jobs);
    js.at("horizon").get_to(instance.horizon);
    js.at("nb_scenarios").get_to(instance.nb_scenarios);
    js.at("nb_hectars").get_to(instance.nb_hectars);
    js.at("GESMAX").get_to(instance.GESMAX);

    auto data_c = js.at("cultures");
    for (json json_culture : data_c){
        Culture culture = Culture();
        nlohmann::from_json(json_culture,culture);
        instance.cultures.push_back(culture);
    }
    auto data_s = js.at("scenarios");
    for (json json_scenario : data_s){
        Scenario scenario = Scenario();
        nlohmann::from_json(json_scenario,scenario);
        instance.scenarios.push_back(scenario);
    }


}
std::ostream& operator<<(std::ostream& os, const Instance& instance)
{   os <<std::endl;
    os << "id : " << instance.id << std::endl;
    os << "nb_jobs : " << instance.nb_jobs << std::endl;
    os << "horizon : " << instance.horizon << std::endl;
    os << "nb_scenarios : " << instance.nb_scenarios << std::endl;
    os << "nb_hectars : " << instance.nb_hectars << std::endl;
    os << "GESMAX : " << instance.GESMAX << std::endl;
    os << "cultures : "<<std::endl;
    for (auto c : instance.cultures)
        os <<c <<std::endl;
    os << "scenarios : "<<std::endl;
    for (auto s : instance.scenarios)
        os << s <<std::endl;
    return os;
}