cmake_minimum_required(VERSION 3.24)
project(PRD_22_Production_agricole)


set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
add_executable(PRD_22_Production_agricole src/main.cpp src/models/Instance.cpp src/models/Instance.h src/models/Culture.cpp src/models/Culture.h src/models/Scenario.cpp src/models/Scenario.h src/Solver.cpp src/Solver.h src/models/Solution.cpp src/models/Solution.h src/models/ConstraintViolationException.cpp src/models/ConstraintViolationException.h)
target_link_libraries(PRD_22_Production_agricole PRIVATE nlohmann_json::nlohmann_json)
