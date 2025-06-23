#ifndef PROBABILITY_PREDICTOR_H
#define PROBABILITY_PREDICTOR_H

#include "DataManager.h"
#include <string>
#include <memory>
#include <algorithm> // For std::min, std::max

class ProbabilityPredictor {
public:
    ProbabilityPredictor(std::shared_ptr<DataManager> dm);

    // Predicts confirmation probability based on historical data
    double getConfirmationProbability(
        const std::string& trainNumber,
        const std::string& trainClass,
        int currentWL,
        int daysToJourney // Days until journey (e.g., 7 for a week away)
    );

private:
    std::shared_ptr<DataManager> dataManager;

    // Simple linear interpolation function (if needed for values not in data)
    double interpolate(double x, double x1, double y1, double x2, double y2);
};

#endif // PROBABILITY_PREDICTOR_H