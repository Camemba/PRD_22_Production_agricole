#include <iostream>
#include <fstream>
#include "Instance.h"

Instance buildInstance(std::string jsonFilePath)
{
    std::ifstream file(jsonFilePath);
    json data = json::parse(file);
    Instance instance = Instance();
    Instance::from_json(data,instance);
    return instance;
}
void exportInstance(std::string jsonFilePath, Instance instance)
{
    json json1;
    nlohmann::to_json(json1,instance);
    std::cout << json1 << std::endl;
    std::ofstream myfile;
    myfile.open (jsonFilePath);
    myfile << json1;
    myfile.close();
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::string jsonFile = "Instances\\generatedInstance1.json";

    Instance instance = buildInstance(jsonFile);
    std::cout<<instance;
    return 0;
}
