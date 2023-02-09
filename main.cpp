#include <iostream>
#include <fstream>
#include "Instance.h"
#include "Solver.h"

Instance buildInstance(std::string jsonFilePath)
{
    std::ifstream file(jsonFilePath);
    json data = json::parse(file);
    Instance instance = Instance();
    Instance::BuildFromJson(data, instance);
    return instance;
}
void exportInstance(std::string jsonFilePath, Instance instance)
{
    json j;
    nlohmann::to_json(j,instance);
    std::cout << j << std::endl;
    std::ofstream myfile;
    myfile.open (jsonFilePath);
    myfile << j;
    myfile.close();
}
void simpleTest(){
    //std::string jsonFile = "Instances\\instance1.json";
    std::string jsonFile = "Stat\\generated_test_instance0.json";
    Instance instance = buildInstance(jsonFile);
    Solver solver = Solver(instance);
    Solution sol = solver.Heuristique1();
    std::cout<<sol<<std::endl;
    std::cout<<"Resultion time : "<<sol.duration<<" ms"<<std::endl;
}
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
    simpleTest();
    makeStat();
    return 0;
}
