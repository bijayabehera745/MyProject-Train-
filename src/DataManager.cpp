#include "DataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

DataManager::DataManager(const std::string& stationsFile,
                           const std::string& trainsFile,
                           const std::string& confirmationFile) {
    loadStations(stationsFile);
    loadTrains(trainsFile);
    loadConfirmationRecords(confirmationFile);
}

std::vector<std::string> DataManager::splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
        token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
        tokens.push_back(token);
    }
    return tokens;
}

void DataManager::loadStations(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open stations file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header row

    while (std::getline(file, line)) {
        std::vector<std::string> tokens = splitString(line, ',');
        if (tokens.size() == 2) {
            stations.emplace_back(tokens[0], tokens[1]);
        } else {
            std::cerr << "Warning: Malformed line in stations.csv: " << line << std::endl;
        }
    }
    file.close();
    std::cout << "Loaded " << stations.size() << " stations." << std::endl;
}

void DataManager::loadTrains(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open trains file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header row

    while (std::getline(file, line)) {
        std::vector<std::string> tokens = splitString(line, ',');
        if (tokens.size() >= 7) {
            std::string trainNumber = tokens[0];
            std::string trainName = tokens[1];
            std::string source = tokens[2];
            std::string destination = tokens[3];
            int duration = std::stoi(tokens[5]);

            std::map<std::string, double> fares;
            fares["SL"] = std::stod(tokens[6]);
            fares["3A"] = std::stod(tokens[7]);

            trains.emplace_back(trainNumber, trainName, source, destination, duration, fares);
        } else {
            std::cerr << "Warning: Malformed line in trains.csv (expected at least 8 columns including fare): " << line << std::endl;
        }
    }
    file.close();
    std::cout << "Loaded " << trains.size() << " trains." << std::endl;
}

void DataManager::loadConfirmationRecords(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open historical confirmation file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header row

    while (std::getline(file, line)) {
        std::vector<std::string> tokens = splitString(line, ',');
        if (tokens.size() == 5) {
            ConfirmationRecord record;
            record.trainNumber = tokens[0];
            record.trainClass = tokens[1];
            record.currentWL = std::stoi(tokens[2]);
            record.daysToJourney = std::stoi(tokens[3]);
            record.probability = std::stod(tokens[4]);
            confirmationRecords.push_back(record);
        } else {
            std::cerr << "Warning: Malformed line in historical_confirmation.csv: " << line << std::endl;
        }
    }
    file.close();
    std::cout << "Loaded " << confirmationRecords.size() << " confirmation records." << std::endl;
}

Station* DataManager::findStationByCode(const std::string& code) {
    for (auto& station : stations) {
        if (station.code == code) {
            return &station;
        }
    }
    return nullptr;
}