#include "ProbabilityPredictor.h"
#include <iostream>

ProbabilityPredictor::ProbabilityPredictor(std::shared_ptr<DataManager> dm)
    : dataManager(dm) {}

double ProbabilityPredictor::getConfirmationProbability(
    const std::string& trainNumber,
    const std::string& trainClass,
    int currentWL,
    int daysToJourney) {

    double predictedProbability = 0.0;
    int bestMatchCount = 0;
    double sumProbabilities = 0.0;

    // Find relevant records
    std::vector<ConfirmationRecord> relevantRecords;
    for (const auto& record : dataManager->getConfirmationRecords()) {
        if (record.trainNumber == trainNumber && record.trainClass == trainClass) {
            relevantRecords.push_back(record);
        }
    }

    // Sort records by WL for easier interpolation/lookup
    std::sort(relevantRecords.begin(), relevantRecords.end(), [](const ConfirmationRecord& a, const ConfirmationRecord& b) {
        return a.currentWL < b.currentWL;
    });

    if (relevantRecords.empty()) {
        // No historical data for this train/class
        // Defaulting to a conservative probability or indicating unknown
        return 5.0; // Very low default if no data
    }

    // Try to find an exact match or use interpolation
    if (currentWL == 0) {
        return 100.0; // Confirmed or RAC is 100% confirmed
    }

    // Find two points for interpolation: one just below and one just above currentWL
    ConfirmationRecord lowerBound, upperBound;
    bool lowerFound = false;
    bool upperFound = false;

    for (const auto& record : relevantRecords) {
        if (record.currentWL <= currentWL) {
            lowerBound = record;
            lowerFound = true;
        }
        if (record.currentWL >= currentWL && !upperFound) { // Find the first upper bound
            upperBound = record;
            upperFound = true;
        }
        if (lowerFound && upperFound) {
            break; // Found both, no need to continue
        }
    }

    if (lowerFound && upperBound.currentWL == currentWL) {
        // Exact match for WL
        predictedProbability = upperBound.probability;
    } else if (lowerFound && upperFound && lowerBound.currentWL != upperBound.currentWL) {
        // Interpolate between lower and upper bounds
        predictedProbability = interpolate(currentWL,
                                           static_cast<double>(lowerBound.currentWL), lowerBound.probability,
                                           static_cast<double>(upperBound.currentWL), upperBound.probability);
    } else if (lowerFound && !upperFound) {
        // currentWL is higher than all recorded WLs, use the lowest recorded probability
        predictedProbability = relevantRecords.back().probability * 0.5; // Apply a decay
    } else if (!lowerFound && upperFound) {
        // currentWL is lower than all recorded WLs (but not 0), use the highest recorded probability
        predictedProbability = relevantRecords.front().probability * 1.2; // Apply a boost
    } else {
        // Should not happen if relevantRecords is not empty, but as a fallback
        predictedProbability = 5.0;
    }

    // Adjust based on daysToJourney (simple linear adjustment, can be complex)
    // More days to journey generally means higher chance if WL is low
    // Fewer days to journey means lower chance if WL is high
    if (daysToJourney > 7) {
        predictedProbability += (daysToJourney - 7) * 0.5; // Small bonus for more days
    } else if (daysToJourney < 7 && daysToJourney > 0) {
        predictedProbability -= (7 - daysToJourney) * 0.5; // Small penalty for fewer days
    }

    // Ensure probability is within 0-100 range
    return std::max(0.0, std::min(100.0, predictedProbability));
}


double ProbabilityPredictor::interpolate(double x, double x1, double y1, double x2, double y2) {
    if (x1 == x2) return y1; // Avoid division by zero
    return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
}