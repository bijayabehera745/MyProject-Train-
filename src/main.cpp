#include "DataManager.h"
#include "TrainSearcher.h"
#include "ProbabilityPredictor.h"
#include "RouteSuggester.h"
#include "UserInterface.h"
#include <iostream>
#include <chrono>
#include <sstream>    // For Date and Time
#include <string>
#include <string_view>
#include <iomanip>
#include <algorithm> // To use functions like transform
#include <vector>
#include <memory> // For std::shared_ptr

int main() {
    UserInterface::displayWelcomeMessage();


    // DataManager initialization needs relative paths to data files
    std::shared_ptr<DataManager> dataManager = std::make_shared<DataManager>(
        "../../data/stations.csv",
        "../../data/trains.csv",
        "../../data/historical_confirmation.csv"
    );

    if (dataManager->getAllStations().empty() || dataManager->getAllTrains().empty()) {
        UserInterface::displayErrorMessage("Failed to load essential data! Exiting...");
        return 1;
    }

    std::shared_ptr<ProbabilityPredictor> probabilityPredictor = std::make_shared<ProbabilityPredictor>(dataManager);
    std::shared_ptr<TrainSearcher> trainSearcher = std::make_shared<TrainSearcher>(dataManager, probabilityPredictor);
    std::shared_ptr<RouteSuggester> routeSuggester = std::make_shared<RouteSuggester>(dataManager, probabilityPredictor);

    std::string originCode, destinationCode;
    UserInterface::getOriginAndDestination(originCode, destinationCode);

    Station* originStation = dataManager->findStationByCode(originCode);
    Station* destinationStation = dataManager->findStationByCode(destinationCode);

    if (!originStation) {
        UserInterface::displayErrorMessage("Origin station code not found: " + originCode);
        return 1;
    }
    if (!destinationStation) {
        UserInterface::displayErrorMessage("Destination station code not found: " + destinationCode);
        return 1;
    }

    std::string journeyDate = UserInterface::getJourneyDate();
    int currentWL = UserInterface::getWaitingListNumber();
    int days = 7; // Assuming 7 days to journey for simplicity



    std::cout << "\nSearching trains from " << originStation->name
              << " (" << originCode << ") to "
              << destinationStation->name << " (" << destinationCode << ")" << std::endl;
    std::cout << "For date: " << journeyDate << std::endl;
    std::cout << "Current WL: " << currentWL << ", Days to Journey: " << days << std::endl;


    // 1. Find and display direct trains
    std::vector<Train> directTrains = trainSearcher->findDirectTrains(originCode, destinationCode);
    std::map<std::string, double> directTrainProbs;
    for (const auto& train : directTrains) {
        directTrainProbs[train.trainNumber] = probabilityPredictor->getConfirmationProbability(
            train.trainNumber, "3A", currentWL, days); // Assuming 3A class for direct searches
    }
    UserInterface::displayTrains(directTrains, "Direct", directTrainProbs);


    // 2. Find and display alternative routes
    std::vector<TravelPlan> alternativePlans = routeSuggester->findAlternativeRoutes(originCode, destinationCode);
    UserInterface::displayTravelPlans(alternativePlans);

    std::cout << "\nSearch complete." << std::endl;

    return 0;
}