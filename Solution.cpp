//
// Created by Bastien Camembert on 11/01/2023.
//

#include "Solution.h"


/**
 * Init methode for water and land availability
 */
void Solution::InitList() {
    int iterOnWater;

    landAtT = std::vector<float>(instance.nbWeeks,  (float)instance.amountLands);
    waterAtT = std::vector<float>(scenario.apport_hebdomadaire);

    waterAtT[0]+= scenario.apport_initial;
    for (iterOnWater = 1; iterOnWater < waterAtT.size(); iterOnWater++)
        waterAtT[iterOnWater] += waterAtT[iterOnWater - 1];


}

Solution::Solution(Instance i, Scenario s): instance(std::move(i)),scenario(std::move(s)), score(0)
        ,greenhouseGasEmission(0), affectedQuantity(std::map<Culture,std::vector<std::pair<int,float>>>()) {
    InitList();
}

Solution::Solution():score(0), greenhouseGasEmission(0){}

/**
 * Stream output operator
 * @param os output
 * @param solution input
 * @return os
 */
std::ostream &operator<<(std::ostream &os, const Solution &solution) {
    os<<std::endl;
    os << "instance: "  << solution.instance.id     <<std::endl;
    os << "score: "     << solution.score           <<std::endl;
    os << "emission: "  << solution.greenhouseGasEmission<<std::endl;

    //os << " Scenario: "    << solution.scenario           <<std::endl;
    /*
    os << "land at T: " <<std::endl;
    int i=0;
    for(auto iland = solution.landAtT.begin();iland<solution.landAtT.end();iland++){
        os<<i<<" :"<<*iland<<"| ";
        i++;
    }
    i=0;
    os <<std::endl<< " water at T: " <<std::endl;
    for(auto iwater = solution.waterAtT.begin();iwater<solution.waterAtT.end();++iwater){
        os<<i<<" :"<<*iwater<<"| ";
        i++;
    }
     */
    os<<std::endl;
    return os;
}


/**
 * Add crop to the solution and update land and water availability
 * @param crop Copy of the crop object
 * @param quantity Amount of crop planted
 * @param start Planting week date
 * @param displayChoice Flag to display when a choice is made (default = false)
 */
void Solution::AllocateCrop(const Culture& crop, float quantity, int start, bool displayChoice) {

    int week ; //update availability iterator

    //crop infos
    float reward = crop.rendement;
    float waterNeeds = crop.besoin_eau;
    float landNeeds = crop.hectars_pour_tonne;
    int cropId = crop.id;
    int growthDuration = crop.duree_pousse;
    float greenhouseGas = crop.emission;
    std::string cropName = crop.nom;

    greenhouseGasEmission += greenhouseGas*quantity;
    score += reward*quantity; //update score
    affectedQuantity[crop].emplace_back(start,quantity);// memorize the planting choice

    if(displayChoice){
        std::cout<<"Affectation de la culture : "<<cropId <<" : "<<cropName<<std::endl;
        std::cout<<"Quantity : "<<quantity <<" date :  "<<start<<std::endl;
        std::cout<<"Current score : "<<score<<std::endl;
    }

    //Update availability
    for (week = start; week < start + growthDuration; week ++){
        landAtT[week]-= landNeeds * quantity;
        waterAtT[week] -=  waterNeeds * quantity; //ASSOCIATE WATER NEEDS AND QUANTITY
    }


}

