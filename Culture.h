//
// Created by camem on 03/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_CULTURE_H
#define PRD_22_PRODUCTION_AGRICOLE_CULTURE_H
#include <string>

class Culture {
public:
    std::string nom;// plus simple pour lire la solution
    float besoin_eau; //besoin en eau par semaine
    int duree_pousse; //nb de semaine nécessaire
    float hectars_pour_tonne; //surface nécessaire pour faire une tonne de culture
    float rendement; //recompense par tonne de la culture
    int depart_tot; //semaine de plantation au plus tôt
    int depart_tard; //semaine de plantation au plus tard
    float emission; //indice de pollution par tonne de la culture

};


#endif //PRD_22_PRODUCTION_AGRICOLE_CULTURE_H
