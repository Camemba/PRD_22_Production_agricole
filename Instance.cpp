//
// Created by camem on 03/01/2023.
//

#include "Instance.h"

Instance::Instance() :id(1),nb_jobs(5),horizon(30),nb_scenarios(3),
        nb_hectars(80), GESMAX(800), cultures({Culture()}), scenarios({Scenario()}) {}

