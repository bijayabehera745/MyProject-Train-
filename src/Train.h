#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <vector>
#include <map>

struct Train {
    std::string trainNumber;
    std::string trainName;
    std::string sourceStationCode;
    std::string destinationStationCode;
    int travelDurationHours;
    std::map<std::string, double> fares; // Class (e.g., "SL", "3A") -> Fare
    std::vector<std::string> intermediateStations;

    Train() : trainNumber(""), trainName(""), sourceStationCode(""), destinationStationCode(""), travelDurationHours(0) {}

    Train(const std::string& num, const std::string& name, const std::string& src,
          const std::string& dest, int duration,
          const std::map<std::string, double>& f,const std::vector<std::string>& routestations)
        : trainNumber(num), trainName(name), sourceStationCode(src),
          destinationStationCode(dest), travelDurationHours(duration), fares(f),intermediateStations(routestations) {}
};

struct RouteSegment {
    std::string trainNumber;
    std::string startStationCode;
    std::string endStationCode;
    int travelDurationHours;

    RouteSegment(const std::string& tn, const std::string& ss, const std::string& es, int td)
        : trainNumber(tn), startStationCode(ss), endStationCode(es), travelDurationHours(td) {}
};

struct TravelPlan {
    std::vector<RouteSegment> segments;
    std::string origin;
    std::string destination;
    int totalTravelTimeHours;
    double totalFare;
    double overallConfirmationProbability;

    TravelPlan() : totalTravelTimeHours(0), totalFare(0.0), overallConfirmationProbability(0.0) {}
};

#endif // TRAIN_H