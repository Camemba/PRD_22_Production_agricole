//
// Created by Bastien Camembert on 11/01/2023.
//

#include "Solution.h"


/**
 * Init methode for water availability
 */
std::vector<float>  Solution::InitWaterAvailability(const Scenario& scenario) {
    int iterOnWater;
    auto water = std::vector<float>(scenario.apport_hebdomadaire);
    //water on week j rely on previous weeks water
    water[0]+= scenario.apport_initial;
    for (iterOnWater = 1; iterOnWater < water.size(); iterOnWater++)
        water[iterOnWater] += water[iterOnWater - 1];
    return water;

}
/**
 * Build a solution from an instance and a scenario
 * @param i copy of input instance
 * @param s copy of input scenario
 */
Solution::Solution(Instance i, Scenario s): instance(std::move(i)),scenario(std::move(s)), score(0),
            duration(0), greenhouseGasEmission(0), affectedQuantity(std::map<Culture,std::map<int,float>>()) {
    landAtT = std::vector<float>(instance.nbWeeks,  (float)instance.amountLands);
    waterConsumption = std::vector<float>(instance.nbWeeks,  0);
    waterAtT = InitWaterAvailability(scenario);
    startPoint = std::chrono::steady_clock::now();
}
/**
 * Default constructor init score and greenhouseGas to 0
 */
Solution::Solution():score(0), greenhouseGasEmission(0), duration(0){}

void Solution::end() {
    auto end = std::chrono::steady_clock::now();
    duration = (float)std::chrono::duration_cast<std::chrono::milliseconds>(end - startPoint).count();
}



/**
 * Display all solution data in stream output operator
 * @param os output address
 */
void Solution::CompleteDisplay(std::ostream* os){
    *os << " Instance: "    << instance <<std::endl;
    *os << " Scenario: "    << scenario <<std::endl;

    *os << "land at T: " <<std::endl;
    int i=0;
    for(auto iterOnLand = landAtT.begin(); iterOnLand < landAtT.end(); iterOnLand++){
        *os << i << " :" << *iterOnLand << "| ";
        i++;

    }
    i=0;
    *os <<std::endl<< " water at T: " <<std::endl;
    for(auto iterOnWater = waterAtT.begin(); iterOnWater < waterAtT.end(); ++iterOnWater){
        *os << i << " :" << *iterOnWater << "| ";
        i++;
    }
    i=0;
    *os <<std::endl<< " water consumption: " <<std::endl;
    for(auto iterOnWater = waterConsumption.begin(); iterOnWater < waterConsumption.end(); ++iterOnWater){
        *os << i << " :" << *iterOnWater << "| ";
        i++;
    }

}/**
 * Add the solution decision data to the output stream
 * @param os output stream address
 * @param solution Solution object reference
 */
void ShowDecision(std::ostream* os, const Solution &solution){
    *os << "Decision : {"<<std::endl;
    auto mapIterator = solution.affectedQuantity.begin();
    for (mapIterator = solution.affectedQuantity.begin(); mapIterator != solution.affectedQuantity.end(); mapIterator++){
        *os << "  *  Culture :"<<mapIterator->first.id<<"("<<mapIterator->first.nom<<")"<<std::endl;
        *os <<"\t";
        for(auto subMapIterator : mapIterator->second){
            *os << "(" << subMapIterator.first << " : " << subMapIterator.second << ")  ";
        }
        *os<<std::endl;
    }
    *os<< "}"<<std::endl;
}

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
    os << "resolution time: "  << solution.duration<<" ms"<<std::endl;
    //CompleteDisplay(&os,solution);
    ShowDecision(&os,solution);
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
    int lastWeek;
    float consoCumul;

    //crop infos
    float reward = crop.rendement;
    float waterNeeds = crop.besoin_eau;
    float landNeeds = crop.hectars_pour_tonne;
    int cropId = crop.id;
    int growthDuration = crop.duree_pousse;
    float greenhouseGas = crop.emission;
    std::string cropName = crop.nom;

    greenhouseGasEmission += greenhouseGas*quantity; //total greenhouse gas emissions
    score += reward*quantity; //update score
    affectedQuantity[crop][start]+=quantity;// memorize the planting choice


    consoCumul = 0;
    //Update availability
    for (week = start; week < start + growthDuration; week ++){
        consoCumul+= waterNeeds ;
        landAtT[week]-= landNeeds * quantity;
        waterConsumption[week] +=consoCumul;
        waterAtT[week] -= consoCumul ;
        lastWeek = week;
    }


    for(week = lastWeek; week <waterAtT.size();week++){
        waterAtT[week] -= consoCumul;
    }

    if(displayChoice){
        std::cout<<"Affectation de la culture : "<<cropId <<" : "<<cropName<<std::endl;
        std::cout<<"Quantity : "<<quantity <<" date :  "<<start<<std::endl;
        std::cout<<"Current score : "<<score<<std::endl;
        std::cout<<std::endl;
    }



}
/**
 * Throw a ConstraintViolationException if the Greenhouse gas emission threshold is exceeded
 */
void Solution::VerifyGGE() const{
    if(greenhouseGasEmission>instance.maxGreenhouseGases)
        throw ConstraintViolationException(Constraint::GGE);
}
/**
 * Add a quantity on a vector from index[start] to index[start+duration]
 * @param consumption vector reference
 * @param start start index
 * @param duration accumulation duration
 * @param quantity accumulation quantity
 */
void AccumulateConsumption(std::vector<float>& consumption, int start,int duration, float quantity){
    int week;
    for(week = start;week<start+duration;week++){
        consumption[week]+= float(week-start+1)*quantity;
    }
}
/**
 * Create a vector with water consumption for each week, from solution decisions
 * @param sol solution object reference
 * @return consumption vector
 */
std::vector<float> CreateWaterConsumptionList(const Solution& sol){
    std::map<int,float> plantationDates;
    int duration;
    float needs;


    auto cons = std::vector<float>(sol.instance.nbWeeks,0);
    auto decisionMap = sol.affectedQuantity;

    for(auto & iterCultureMap : decisionMap){
        plantationDates = iterCultureMap.second;
        duration = iterCultureMap.first.duree_pousse;
        needs = iterCultureMap.first.besoin_eau;
        for(auto & plantationDate : plantationDates){
            AccumulateConsumption(cons, plantationDate.first, duration,  needs);
        }
    }

    return cons;
}

/**
 * Verify if the water consumption constraint is respected return an exception otherwise
 * @throw ConstraintViolationException
 */
void Solution::VerifyWaterConsumption(){
    auto availability = std::vector<std::vector<float>>(); //water availability for each scenario
    int iterOnScenario;
    int week ;
    std::vector<float> water;
    auto cons = CreateWaterConsumptionList(*this);

    //Init availability list
    for(const Scenario& s: instance.scenarios){
        availability.push_back(InitWaterAvailability(s));
    }
    for(iterOnScenario = 0;iterOnScenario<availability.size();iterOnScenario++){
        water = availability[iterOnScenario];
        for(week =0; week<instance.nbWeeks;week++){
            if (water[week]<waterConsumption[week])
                throw ConstraintViolationException(Constraint::Water,iterOnScenario,week);
        }
    }
}
/**
 * Verify if the land consumption constraint is respected return an exception otherwise
 * @throw ConstraintViolationException
 */
void Solution::VerifyLandConsumption(){
    int week ;
    auto landAvailable = (float)instance.amountLands;
        for(week =0; week<instance.nbWeeks;week++){
            if (landAvailable<landAtT[week])
                throw ConstraintViolationException(Constraint::Land,-1,week);
        }
}


/**
 * Verify solution feasibility and identify which constraint isn't respected
 * Display the result if parameter is false, throw exception otherwise
 * @param throwException choose between display or throw exception
 */
void Solution::Verify(bool throwException=false){
    try{
        VerifyWaterConsumption();
        VerifyLandConsumption();
        VerifyGGE();

        if(!throwException)
            std::cout<<"Feasible Solution"<<std::endl;
    }catch (ConstraintViolationException& cve){
        if(throwException)
            throw cve;
        else
            std::cout<<"Non-Feasible Solution : "<<cve.what()<<std::endl;
    }
}