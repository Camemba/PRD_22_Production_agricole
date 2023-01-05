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

    //TODO transfomer data en vector<culture>
    auto data = js.at("cultures");
    for (json culture : data){
        //std::cout << culture;
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
    return os;
}