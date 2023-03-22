//
// Created by Bastien Camembert on 15/02/2023.
//

#include "ConstraintViolationException.h"

/**
 * Return emission constraint violation message
 * @return exception message
 */
char * throwGasEmissionException(){
    return (char*)"Greenhouse gas emissions threshold exceeded";
}
/**
 * Return water consumption constraint violation message
 * @param scenario scenario index where constraint is not respected
 * @param week week index where constraint is not respected
 * @return exception message
 */
char * throwWaterConsumptionException(int scenario, int week){
    std::string str = "Water consumption exceed availability at Scenario: ";
    str + std::to_string(scenario) + " week: "+std::to_string(week);
    return const_cast<char *>(str.c_str());
}
/**
 * Return land consumption constraint violation message
 * @param week week index where constraint is not respected
 * @return exception message
 */
char * throwLandConsumptionException(int week){
    std::string str = "Land needs exceed availability at week: "+std::to_string(week);
    return const_cast<char *>(str.c_str());
}


ConstraintViolationException::ConstraintViolationException(Constraint constraint, int scenario, int week)
: scenario(scenario),constraint(constraint),week(week){}


/**
 * Determine which message exception has to return depending on constraint type
 * @return exception message
 */
char * ConstraintViolationException::what() {
    if(constraint == Constraint::GGE)
        return throwGasEmissionException();
    if(constraint == Constraint::Land)
        return throwLandConsumptionException(week);
    if(constraint == Constraint::Water)
        return throwWaterConsumptionException(scenario,week);
    return nullptr;
}

