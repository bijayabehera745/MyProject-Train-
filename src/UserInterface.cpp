//Done
#include "UserInterface.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <algorithm> //To use functions like transform

void UserInterface::displayWelcomeMessage() {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "        Train Ticket Confirmation Predictor" << std::endl;
    std::cout << "                                   By Bijay" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
}

void UserInterface::getOriginAndDestination(std::string& origin, std::string& destination) {
    std::cout << "Enter Origin Station Code (e.g., HWH): ";
    std::cin >> origin;
    std::cout << "Enter Destination Station Code (e.g., BBS): ";
    std::cin >> destination;
    // Convert to uppercase for consistency
    std::transform(origin.begin(), origin.end(), origin.begin(), ::toupper);
    std::transform(destination.begin(), destination.end(), destination.begin(), ::toupper);
}

std::string UserInterface::getJourneyDate() {
    std::string dateStr;
    std::cout << "Enter Journey Date (DDMMYYYY): ";
    std::cin >> dateStr;
    return dateStr;
}

int UserInterface::getWaitingListNumber() {
    int wl;
    std::cout << "Enter current Waiting List WL number (0 if confirmed/RAC): ";
    std::cin >> wl;
    return wl;
}

void UserInterface::displayTrains(const std::vector<Train>& trains, const std::string& type,
                                   const std::map<std::string, double>& probabilities) {
    if (trains.empty()) {
        std::cout << "No " << type << " trains found." << std::endl;
        return;
    }

    std::cout << "\n--- " << type << " Trains ---" << std::endl;
    std::cout << std::left << std::setw(15) << "Train No.  |"
              << std::setw(30) << "Train Name  |"
              << std::setw(10) << "Source  |"
              << std::setw(10) << "Dest.  |"
              << std::setw(10) << "Duration  |"
              << std::setw(15) << "Fare (SL)  |"
              << std::setw(15) << "Fare (3A)  |"
              << std::setw(15) << "Conf. Prob.  |" << std::endl;
    std::cout << std::string(110, '-') << std::endl;

    for (const auto& train : trains) {
        double prob = 0.0;
        if (probabilities.count(train.trainNumber)) {
            prob = probabilities.at(train.trainNumber);
        }

        std::cout << std::left << std::setw(15) << train.trainNumber
                  << std::setw(30) << train.trainName
                  << std::setw(10) << train.sourceStationCode
                  << std::setw(10) << train.destinationStationCode
                  << std::setw(10) << train.travelDurationHours
                  << std::setw(15) <<(train.fares.count("SL") ? std::to_string(train.fares.at("SL")) : "NA")
                  << std::setw(15) <<(train.fares.count("3A") ? std::to_string(train.fares.at("3A")) : "NA")
                  << std::fixed << std::setprecision(2) << prob << "%" << std::endl;
    }
    std::cout << std::endl;
}

void UserInterface::displayTravelPlans(const std::vector<TravelPlan>& plans) {
    if (plans.empty()) {
        std::cout << "No alternative routes found." << std::endl;
        return;
    }

    std::cout << "\n--- Alternative Travel Plans ---" << std::endl;
    for (size_t i = 0; i < plans.size(); ++i) {
        const auto& plan = plans[i];
        std::cout << "\nPlan " << (i + 1) << ":" << std::endl;
        std::cout << "  Total Travel Time: " << plan.totalTravelTimeHours << " hours" << std::endl;
        std::cout << "  Total Estimated Fare: Rs. " << plan.totalFare << std::endl;
        std::cout << "  Overall Confirmation Probability: " << std::fixed << std::setprecision(2) << plan.overallConfirmationProbability << "%" << std::endl;
        std::cout << "  Route: ";
        for (size_t j = 0; j < plan.segments.size(); ++j) {
            const auto& segment = plan.segments[j];
            std::cout << segment.startStationCode << " --[" << segment.trainNumber << "]--> " << segment.endStationCode;
            if (j < plan.segments.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void UserInterface::displayErrorMessage(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
}