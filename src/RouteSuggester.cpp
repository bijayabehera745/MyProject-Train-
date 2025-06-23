#include "RouteSuggester.h"
#include <queue>
#include <set>
#include <limits>
#include <iostream>

RouteSuggester::RouteSuggester(std::shared_ptr<DataManager> dm, std::shared_ptr<ProbabilityPredictor> pp)
    : dataManager(dm), probabilityPredictor(pp) {}

std::vector<TravelPlan> RouteSuggester::findAlternativeRoutes(const std::string& originCode,
                                                          const std::string& destinationCode,
                                                          int maxHops) {
    std::vector<TravelPlan> alternativePlans;
    std::queue<BFSNode> q;
    std::set<std::string> visitedStations; // To prevent infinite loops in cycles or redundant paths

    q.push({originCode, {}, 0, 0, 0.0});
    // Note: For multi-hop, a simple 'visited' set at BFSNode pushing can sometimes be too restrictive.
    // More advanced graph algorithms for paths often use ways to avoid re-visiting *in the same path segment*,
    // or prioritize based on cost/time. For simplicity here, we use a basic visited set.
    // A more robust solution might need to track (station, hops) or (station, current_path_hash)

    while (!q.empty()) {
        BFSNode current = q.front();
        q.pop();

        // Optimization: If current station is already the destination, add plan if valid
        if (current.currentStationCode == destinationCode) {
            TravelPlan plan;
            plan.segments = current.path;
            plan.origin = originCode;
            plan.destination = destinationCode;
            plan.totalTravelTimeHours = current.currentTravelTime;
            plan.totalFare = current.currentFare;

            // Calculate overall confirmation probability for the multi-segment route
            // This is a simplification: multiplying probabilities assumes independence, which isn't strictly true for real train travel.
            // A better model would involve more complex joint probability or a more detailed historical model.
            double combinedProb = 100.0;
            if (!plan.segments.empty()) {
                for (const auto& segment : plan.segments) {
                    // Using dummy WL=10, DaysToJourney=7 for alternative routes for demonstration
                    // In a real system, you'd prompt for WL for each segment or use an average/default.
                    double segmentProb = probabilityPredictor->getConfirmationProbability(
                                            segment.trainNumber, "3A", 10, 7);
                    combinedProb *= (segmentProb / 100.0); // Convert to decimal for multiplication
                }
                combinedProb *= 100.0; // Convert back to percentage
            } else {
                // This case implies a direct connection somehow ended up here without segments.
                // Or it's a fallback if no segments were added, which shouldn't happen for valid plans.
                combinedProb = 100.0;
            }
            plan.overallConfirmationProbability = std::max(0.0, std::min(100.0, combinedProb));
            alternativePlans.push_back(plan);
            continue; // Found a path to destination, continue to next queue item
        }

        // Only explore further if within maxHops
        if (current.hops >= maxHops) {
            continue;
        }

        // Explore trains departing from the current station
        for (const auto& train : dataManager->getAllTrains()) {
            if (train.sourceStationCode == current.currentStationCode) {
                // Create a new segment for this train
                RouteSegment newSegment(train.trainNumber,
                                        train.sourceStationCode,
                                        train.destinationStationCode,
                                        train.travelDurationHours);

                // Create a new path by adding the new segment
                std::vector<RouteSegment> newPath = current.path;
                newPath.push_back(newSegment);

                int newTravelTime = current.currentTravelTime + train.travelDurationHours;
                double newFare = current.currentFare;
                if (train.fares.count("3A")) { // Use 3A fare for alternative routes
                    newFare += train.fares.at("3A");
                } else if (train.fares.count("SL")) { // Fallback to SL if 3A not available
                     newFare += train.fares.at("SL");
                }
                // Else, fare remains 0 or needs a default/error handling

                // Add to queue for further exploration. No 'visited' check based on just station code
                // because we want to allow re-visiting a station on a different path or hop count.
                // A proper pathfinding algorithm might need to check (station, current_path_signature)
                // or (station, current_cost). For BFS, just tracking hops limits cycles.
                q.push({train.destinationStationCode, newPath, current.hops + 1, newTravelTime, newFare});
            }
        }
    }

    return alternativePlans;
}