//
// Created by camem on 15/02/2023.
//

#ifndef INSTANCEGENERATOR_PY_CONSTRAINTVIOLATIONEXCEPTION_H
#define INSTANCEGENERATOR_PY_CONSTRAINTVIOLATIONEXCEPTION_H


#include <iostream>

class ConstraintViolationException : public std::exception{
private:
    char * message;

public:
    ConstraintViolationException(char * msg) : message(msg) {}
    char * what () {
        return message;
    }
};


#endif //INSTANCEGENERATOR_PY_CONSTRAINTVIOLATIONEXCEPTION_H
