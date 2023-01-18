 //
// Created by camem on 11/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_SOLUTION_H
#define PRD_22_PRODUCTION_AGRICOLE_SOLUTION_H


#include <vector>
#include <map>
#include "Culture.h"
#include "Scenario.h"
#include "Instance.h"

 class Solution {
public:

    Instance instance; // Copie de l'instance
    Scenario scenario; // Scenario sur lequel se base la solution

    std::vector<float> landAtT; // Terre disponible a l'instant t
    std::vector<float> waterAtT; // eau disponible a l'instant t
    float score;

    /*
     * Structure de donnee pour stocker l'affectation d'une culture
     * On considere qu'il est possible de planter plusieurs fois la meme
     * culture a des dates de plantation differentes et en quantite differente
     */
    std::map<Culture,std::vector<std::pair<int,float>>> affectedQuantity;

public:
     Solution();
     Solution(Instance i, Scenario s);

     void AllocateCrop(Culture crop, float quantity, int start);
private:
    void InitList();

};


#endif //PRD_22_PRODUCTION_AGRICOLE_SOLUTION_H
