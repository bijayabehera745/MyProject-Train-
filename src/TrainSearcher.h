#ifndef TRAIN_SEARCHER_H
#define TRAIN_SEARCHER_H

#include "DataManager.h"
#include "Train.h"
#include "ProbabilityPredictor.h"
#include <vector>
#include <string>
#include <memory>

class TrainSearcher {
public:
    TrainSearcher(std::shared_ptr<DataManager> dm, std::shared_ptr<ProbabilityPredictor> pp);

    std::vector<Train> findDirectTrains(const std::string& originCode,
                                        const std::string& destinationCode);

private:
    std::shared_ptr<DataManager> dataManager;
    std::shared_ptr<ProbabilityPredictor> probabilityPredictor;
};

#endif // TRAIN_SEARCHER_H