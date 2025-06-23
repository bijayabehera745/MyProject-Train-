#include "TrainSearcher.h"
#include <iostream>
#include <algorithm> // For std::transform
#include <cctype>    // For std::toupper

TrainSearcher::TrainSearcher(std::shared_ptr<DataManager> dm, std::shared_ptr<ProbabilityPredictor> pp)
    : dataManager(dm), probabilityPredictor(pp) {}

std::vector<Train> TrainSearcher::findDirectTrains(const std::string& originCode,
                                                  const std::string& destinationCode) {
    std::vector<Train> foundTrains;
    
    std::string upperOriginCode = originCode;
    std::string upperDestinationCode = destinationCode;
    std::transform(upperOriginCode.begin(), upperOriginCode.end(), upperOriginCode.begin(), ::toupper);
    std::transform(upperDestinationCode.begin(), upperDestinationCode.end(), upperDestinationCode.begin(), ::toupper);
    
    // --- NEW DEBUG PRINT ---
    std::cout << "\nDEBUG: Searching for Origin: " << upperOriginCode
              << ", Destination: " << upperDestinationCode << "\n";
    // --- END NEW DEBUG PRINT ---

    for (const auto& train : dataManager->getAllTrains()) {
        
        // --- NEW DEBUG PRINT ---
        std::cout << "DEBUG: Checking Train: " << train.trainNumber << " (" << train.trainName << ") -> Route: [";
        for (size_t i = 0; i < train.intermediateStations.size(); ++i) {
            std::cout << train.intermediateStations[i];
            if (i < train.intermediateStations.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
        // --- END NEW DEBUG PRINT ---

        //Find the origin station in the train's route
        auto itOrigin = std::find(train.intermediateStations.begin(), train.intermediateStations.end(), upperOriginCode);
        // Find the destination station in the train's route
        auto itDestination = std::find(train.intermediateStations.begin(),train.intermediateStations.end(), upperDestinationCode);

        // --- NEW DEBUG PRINT ---
        bool originFound = (itOrigin != train.intermediateStations.end());
        bool destFound = (itDestination != train.intermediateStations.end());
        bool inOrder = (originFound && destFound && (itOrigin < itDestination));
        std::cout << "DEBUG:   Origin '" << upperOriginCode << "' Found: " << (originFound ? "Yes" : "No")
                  << ", Destination '" << upperDestinationCode << "' Found: " << (destFound ? "Yes" : "No")
                  << ", In Order: " << (inOrder ? "Yes" : "No") << "\n";
        // --- END NEW DEBUG PRINT ---

        if (originFound && destFound && inOrder)
        {
            foundTrains.push_back(train);
            std::cout << "DEBUG:   --> Adding train " << train.trainNumber << ".\n"; // Indicate when a train is added
        }
    }
    return foundTrains;
}