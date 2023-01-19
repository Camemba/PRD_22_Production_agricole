 //
// Created by Bastien Camembert on 11/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_SOLUTION_H
#define PRD_22_PRODUCTION_AGRICOLE_SOLUTION_H


#include <map>
#include <utility>
#include "Instance.h"

 class Solution {
public:

    Instance instance; // Instance copy
    Scenario scenario; // Scenario copy where the solution takes place

    std::vector<float> landAtT; // land availability at t
    std::vector<float> waterAtT; // water availability at t
    float score;

    /*
     * Data structure to store a crop allocation
     * It considers a crop to be planted several times at different starts and different quantities
     */
    std::map<Culture,std::vector<std::pair<int,float>>> affectedQuantity;

public:
     Solution();
     Solution(Instance i, Scenario s);

     void AllocateCrop(const Culture& crop, float quantity, int start, bool displayChoice=false);
private:
    void InitList();

};


#endif //PRD_22_PRODUCTION_AGRICOLE_SOLUTION_H
