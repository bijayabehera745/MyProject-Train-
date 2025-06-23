#ifndef ROUTE_SUGGESTER_H
#define ROUTE_SUGGESTER_H

#include "DataManager.h"
#include "Train.h"
#include "ProbabilityPredictor.h"
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <map>

class RouteSuggester {
public:
    RouteSuggester(std::shared_ptr<DataManager> dm, std::shared_ptr<ProbabilityPredictor> pp);

    std::vector<TravelPlan> findAlternativeRoutes(const std::string& originCode,
                                                  const std::string& destinationCode,
                                                  int maxHops = 2);

private:
    std::shared_ptr<DataManager> dataManager;
    std::shared_ptr<ProbabilityPredictor> probabilityPredictor;

    struct BFSNode {
        std::string currentStationCode;
        std::vector<RouteSegment> path;
        int hops;
        int currentTravelTime;
        double currentFare;
    };
};

#endif // ROUTE_SUGGESTER_H