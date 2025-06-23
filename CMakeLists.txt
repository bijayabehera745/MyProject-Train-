cmake_minimum_required(VERSION 3.10) # Or a newer version you have, like 3.15, 3.20, etc.
project(train_predictor CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify source files
set(SOURCE_FILES
    src/main.cpp
    src/Station.cpp
    src/Train.cpp
    src/DataManager.cpp
    src/TrainSearcher.cpp
    src/ProbabilityPredictor.cpp
    src/RouteSuggester.cpp
    src/UserInterface.cpp
)

# Add executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# Optional: Set output directory for executables (makes it easier to find)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)