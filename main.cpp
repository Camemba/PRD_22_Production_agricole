#include <iostream>
#include <fstream>
#include "Instance.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Instance instance = Instance();
    json json1;
    nlohmann::to_json(json1,instance);
    std::cout << json1 << std::endl;
    std::ofstream myfile;
    myfile.open ("Instances\\instance1.json");
    myfile << json1;
    myfile.close();
    return 0;
}
