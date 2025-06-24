cmake_minimum_required(VERSION 3.15)
project(TrainPredictorConsole CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

# Defined the executable target
add_executable(TrainPredictor
    src/main.cpp             # Your main console entry point
    src/DataManager.cpp
    src/TrainSearcher.cpp
    src/ProbabilityPredictor.cpp
    src/RouteSuggester.cpp
    # Add any other .cpp files your project uses here (e.g., Train.cpp if it's separate)
)

# Specify include directories so the compiler can find your header files
target_include_directories(TrainPredictor PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

# Optional: Set output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")