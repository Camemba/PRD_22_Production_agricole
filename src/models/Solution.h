 //
// Created by Bastien Camembert on 11/01/2023.
//

#ifndef PRD_22_PRODUCTION_AGRICOLE_SOLUTION_H
#define PRD_22_PRODUCTION_AGRICOLE_SOLUTION_H


#include <map>
#include <utility>
#include <ostream>
#include "Instance.h"
#include "ConstraintViolationException.h"

 class Solution {
public:

    Instance instance; // Instance copy
    Scenario scenario; // Scenario copy where the solution takes place

    std::vector<float> landAtT; // land availability at t
    std::vector<float> waterAtT; // water availability at t
    std::vector<float> waterConsumption;
    float score ;
    float greenhouseGasEmission;
    std::chrono::steady_clock::time_point start;
    float duration;


    /*
     * Data structure to store a crop allocation
     * It considers a crop to be planted several times at different starts and different quantities
     */
    std::map<Culture,std::map<int,float>> affectedQuantity;

public:
     Solution();
     Solution(Instance i, Scenario s);

     void end();

     void AllocateCrop(const Culture& crop, float quantity, int start, bool displayChoice=false);
     friend std::ostream &operator<<(std::ostream &os, const Solution &solution);
     void Verify(); // call Verify(false)
     void Verify(bool throwException);

 private:
    std::vector<float> InitWaterAvailability(const Scenario& scenario);
    void VerifyGGE() const;
    void VerifyWaterConsumption();
 };


#endif //PRD_22_PRODUCTION_AGRICOLE_SOLUTION_H
