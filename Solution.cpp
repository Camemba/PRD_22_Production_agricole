//
// Created by camem on 11/01/2023.
//

#include "Solution.h"


void Solution::InitList() {
    landAtT = std::vector<float>(instance.horizon, instance.nb_hectars);
    waterAtT = std::vector<float>(scenario.apport_hebdomadaire);

}

Solution::Solution(Instance i, Scenario s): instance(i),scenario(s)
        , affectedQuantity(std::map<Culture,std::vector<std::pair<int,float>>>()) {
    InitList();
}

Solution::Solution() {}

void Solution::AllocateCrop(Culture crop, float quantity, int start) {
    int week = 0;
    score += crop.rendement*quantity;
    std::cout<<"score : "<<score<<std::endl;
    std::cout<<"Affectation de la culture : "<<crop.id <<" : "<<crop.nom<<std::endl;
    std::cout<<"Quantite : "<<quantity <<" date :  "<<start<<std::endl;
    affectedQuantity[crop].push_back(std::pair(start,quantity));
    //Mise a jour des ressources disponible
    for (week = start; week < start + crop.duree_pousse; week ++){
        landAtT[week]-= crop.hectars_pour_tonne * quantity;
        waterAtT[week] -= crop.besoin_eau * quantity; //ICI LE BESOIN EN EAU DEPEND DE LA QUANTITE
    }


}

