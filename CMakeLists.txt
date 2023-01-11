cmake_minimum_required(VERSION 3.24)
project(PRD_22_Production_agricole)


set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
add_executable(PRD_22_Production_agricole main.cpp Instance.cpp Instance.h Culture.cpp Culture.h Scenario.cpp Scenario.h Solver.cpp Solver.h Solution.cpp Solution.h)
target_link_libraries(PRD_22_Production_agricole PRIVATE nlohmann_json::nlohmann_json)
