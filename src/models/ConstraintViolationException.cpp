//
// Created by Bastien Camembert on 15/02/2023.
//

#include "ConstraintViolationException.h"

char * throwGasEmissionException(){
    return (char*)"Greenhouse gas emissions threshold exceeded";
}
char * throwWaterConsumptionException(int scenario, int week){
    std::string str = "Water consumption exceed availability at Scenario: ";
    str + std::to_string(scenario) + " week: "+std::to_string(week);
    return const_cast<char *>(str.c_str());
}
ConstraintViolationException::ConstraintViolationException(Constraint constraint, int scenario, int week)
: scenario(scenario),constraint(constraint),week(week){}

char * ConstraintViolationException::what() {
    if(constraint == Constraint::GGE)
        return throwGasEmissionException();
    if(constraint == Constraint::Water)
        return throwWaterConsumptionException(scenario,week);
    return 0;
}

