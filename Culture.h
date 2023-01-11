//
// Created by camem on 03/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_CULTURE_H
#define PRD_22_PRODUCTION_AGRICOLE_CULTURE_H
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Culture {
public:
    Culture();

public:
    int id; // id unique de la culture dans le cas de plusieurs varietes
    std::string nom;// plus simple pour lire la solution
    float besoin_eau; //besoin en eau par semaine
    int duree_pousse; //nb de semaine nécessaire
    float hectars_pour_tonne; //surface nécessaire pour faire une tonne de culture
    float rendement; //recompense par tonne de la culture
    int depart_tot; //semaine de plantation au plus tôt
    int depart_tard; //semaine de plantation au plus tard
    float emission; //indice de pollution par tonne de la culture

    bool operator<(const Culture &rhs) const;
    /*
   * Macro pour "JSONiser" la classe
   * TYPE_INTRUSIVE permet d'accèder à des attributs privés
   * Chaque sous-classe utilisée doit etre JSONisable elle aussi
   */
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Culture,id, nom, besoin_eau, duree_pousse,
                                   hectars_pour_tonne, rendement, depart_tot,
                                   depart_tard, emission)
};

// Affichage en sortie standard
std::ostream& operator<<(std::ostream& os, const Culture& culture);

#endif //PRD_22_PRODUCTION_AGRICOLE_CULTURE_H
