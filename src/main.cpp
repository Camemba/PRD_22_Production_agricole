#include <iostream>
#include <fstream>
#include "models/Instance.h"
#include "Solver.h"

/**
 * Create an Instance object from a Json file
 * @param jsonFilePath instance file path
 * @return an Instance object
 */
Instance buildInstance(std::string jsonFilePath)
{
    std::ifstream file(jsonFilePath);
    json data = json::parse(file);
    Instance instance = Instance();
    Instance::BuildFromJson(data, instance);
    return instance;
}
/**
 * Export an Instance object in a JSON file
 * @param jsonFilePath output file path
 * @param instance instance Object
 */
void exportInstance(std::string jsonFilePath, Instance instance)
{
    json j;
    nlohmann::to_json(j,instance);//convert an instance into JSON format thanks to macro in Instance.h
    //std::cout << j << std::endl;
    std::ofstream myfile;
    myfile.open (jsonFilePath);// overwrite the file if exist create it if not
    myfile << j;
    myfile.close();
}

/**
 * Launch the solver on a specified instance and verify solution feasibility
 * @param jsonFile instance file
 */
void simpleTest(std::string jsonFile){
    Instance instance = buildInstance(jsonFile);
    Solver solver = Solver(instance);
    Solution sol = solver.Heuristique1();
    sol.Verify();
    std::cout<<sol<<std::endl;
    std::cout<<"Resultion time : "<<sol.duration<<" ms"<<std::endl;
}
/**
 * Launch solver on multiple instances to make statistics
 */
void makeStat(){
    std::fstream outfile;
    Instance instance;
    Solution sol;
    std::string workingDirectory = "Stat\\generated_test_instance";
    std::string jsonFile;

    int i;
    int cumulate = 0;
    for(i=0; i<90;i++){
        jsonFile = workingDirectory + std::to_string(i) +".json";
        instance = buildInstance(jsonFile);
        Solver solver = Solver(instance);
        sol = solver.Heuristique1();
        outfile.open("result.txt", std::fstream::app); // append instead of overwrite
        outfile << jsonFile <<";"<<sol.score<<";"<<sol.duration<<";"<<std::endl;
        outfile.close();
        cumulate+=sol.duration;
    }
    std::cout<<"Total : "<<cumulate<<" ms"<<std::endl;
}
int main(int argc, char *argv[]) {
    simpleTest("Stat\\generated_test_instance0.json");
    return 0;
}
