#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "Station.h"
#include "Train.h"
#include <vector>
#include <string>
#include <map>

struct ConfirmationRecord {
    std::string trainNumber;
    std::string trainClass;
    int currentWL;
    int daysToJourney;
    double probability;
};

class DataManager {
public:
    DataManager(const std::string& stationsFile,
                const std::string& trainsFile,
                const std::string& confirmationFile);

    const std::vector<Station>& getAllStations() const { return stations; }
    const std::vector<Train>& getAllTrains() const { return trains; }
    const std::vector<ConfirmationRecord>& getConfirmationRecords() const { return confirmationRecords; }

    Station* findStationByCode(const std::string& code);

private:
    std::vector<Station> stations;
    std::vector<Train> trains;
    std::vector<ConfirmationRecord> confirmationRecords;

    void loadStations(const std::string& filename);
    void loadTrains(const std::string& filename);
    void loadConfirmationRecords(const std::string& filename);

    std::vector<std::string> splitString(const std::string& s, char delimiter);
};

#endif // DATA_MANAGER_H