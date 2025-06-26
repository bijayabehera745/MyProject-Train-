#include "DataManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <algorithm> // For std::transform

DataManager::DataManager(const std::string& stationsFile,
                           const std::string& trainsFile,
                           const std::string& confirmationFile) {
    loadStations(stationsFile);
    loadTrains(trainsFile);
    loadConfirmationRecords(confirmationFile);
}

std::vector<std::string> DataManager::splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string currentToken;
    std::istringstream tokenStream(s); 
    bool inQuote = false; // Flag to track if we are inside a double-quoted field

    char c;
    while (tokenStream.get(c)) { // Read character by character
        if (c == '"') {
            inQuote = !inQuote; // Toggle the inQuote flag
            // Do NOT add the quote character to currentToken if it's just for quoting
            // (Assumes quotes are only at the start/end of a field that needs them, not escaped within)
        } else if (c == delimiter && !inQuote) {
            // Found a delimiter, AND we are NOT inside quotes
            // This marks the end of a token
            // Trim whitespace from the token before adding
            currentToken.erase(0, currentToken.find_first_not_of(" \t\n\r\f\v"));
            currentToken.erase(currentToken.find_last_not_of(" \t\n\r\f\v") + 1);
            tokens.push_back(currentToken);
            currentToken.clear(); // Reset for the next token
        } else {
            // It's a regular character, or a delimiter inside quotes
            currentToken += c; // Add character to the current token
        }
    }
     // After the loop, add the last token (since there's no delimiter after it)
    // Trim whitespace from the last token
    currentToken.erase(0, currentToken.find_first_not_of(" \t\n\r\f\v"));
    currentToken.erase(currentToken.find_last_not_of(" \t\n\r\f\v") + 1);
    tokens.push_back(currentToken);
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
        if (tokens.size() >= 9) {
            std::string trainNumber = tokens[0];
            std::string trainName = tokens[1];
            std::string source = tokens[2];
            std::string destination = tokens[3];
            //The token 4 is skipped because running days coloumn is ignored (Asuuming all trains running on all days)
            int duration = std::stoi(tokens[5]);

            std::map<std::string, double> fares;
            fares["SL"] = std::stod(tokens[6]);
            fares["3A"] = std::stod(tokens[7]);
            std::vector<std::string> routeStations;
            std::string routeString = tokens[8];

            //Converting CSV input to vector of station codes
             std::stringstream routeSs(routeString);
                std::string stationCode;
                while (std::getline(routeSs, stationCode, ',')) {
                    // Converted station code to uppercase
                    std::transform(stationCode.begin(), stationCode.end(), stationCode.begin(), ::toupper);
                    routeStations.push_back(stationCode);
                }
            
               
            trains.emplace_back(trainNumber, trainName, source, destination, duration, fares,routeStations);
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