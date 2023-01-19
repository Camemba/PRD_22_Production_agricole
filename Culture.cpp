//
// Created by Bastien Camembert  on 03/01/2023.
//

#include "Culture.h"

Culture::Culture(): id(),nom(), besoin_eau(),
                    duree_pousse(),
                    hectars_pour_tonne(),
                    rendement(), depart_tot(),
                    depart_tard(), emission() {}

bool Culture::operator<(const Culture &rhs) const {
    return id < rhs.id;
}


std::ostream& operator<<(std::ostream& os, const Culture& c)
{
    os << "{id : "<< c.id<< " | nom : " << c.nom <<"\t| rendement : " << c.rendement <<" | besoin en eau : ";
    os <<c.besoin_eau << " | emission : "<< c.emission <<" | surface : " <<c.hectars_pour_tonne;
    os << " | duree : "<<c.duree_pousse <<" | depart : ["<<c.depart_tot<<" - "<<c.depart_tard<<"]}";

    return os;
}