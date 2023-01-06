//
// Created by camem on 03/01/2023.
//

#include "Scenario.h"

Scenario::Scenario() : apport_initial(),apport_hebdomadaire(){}


std::ostream& operator<<(std::ostream& os, const Scenario& s)
{
    os << "{ apport_initial : " << s.apport_initial <<std::endl;
    os <<" apport hebdo : | ";
    for(auto apport : s.apport_hebdomadaire)
        os << apport<<" | ";
    os<<"}";
    return os;
}