#include "TrainSearcher.h"
#include <iostream>

TrainSearcher::TrainSearcher(std::shared_ptr<DataManager> dm, std::shared_ptr<ProbabilityPredictor> pp)
    : dataManager(dm), probabilityPredictor(pp) {}

std::vector<Train> TrainSearcher::findDirectTrains(const std::string& originCode,
                                                  const std::string& destinationCode) {
    std::vector<Train> foundTrains;
    for (const auto& train : dataManager->getAllTrains()) {
        if (train.sourceStationCode == originCode &&
            train.destinationStationCode == destinationCode) {
            foundTrains.push_back(train);
        }
    }
    return foundTrains;
}