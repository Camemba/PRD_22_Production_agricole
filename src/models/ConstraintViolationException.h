//
// Created by Bastien Camembert on 15/02/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_CONSTRAINTVIOLATIONEXCEPTION_H
#define PRD_22_PRODUCTION_AGRICOLE_CONSTRAINTVIOLATIONEXCEPTION_H


#include <iostream>

enum class Constraint {GGE,Water};

class ConstraintViolationException : public std::exception{
private:
    Constraint constraint;
    int scenario;
    int week;

public:
    ConstraintViolationException(Constraint constraint) : constraint(constraint) {}
    ConstraintViolationException(Constraint constraint, int scenario, int week);
    char * what () ;
};

#endif //PRD_22_PRODUCTION_AGRICOLE_CONSTRAINTVIOLATIONEXCEPTION_H
