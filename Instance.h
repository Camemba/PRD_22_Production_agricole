//
// Created by camem on 03/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_INSTANCE_H
#define PRD_22_PRODUCTION_AGRICOLE_INSTANCE_H


class Instance {
public:
    int nb_jobs; //nombre de cultures
    int horizon; //nombre de semaines observées
    int nb_scenarios;
    int HV; //High Value
    int nb_hectars; //quantité de terre disponnible
    float GESMAX; //seuil de pollution maximale

};


#endif //PRD_22_PRODUCTION_AGRICOLE_INSTANCE_H
