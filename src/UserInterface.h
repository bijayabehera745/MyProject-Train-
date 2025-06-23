#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "Station.h"
#include "Train.h"
//"#include <bits/stdc++.h>" Also can be used.
#include <string>
#include <vector>
#include <map>

class UserInterface {
public:
    //This part only defines the type of function and its input parameters
    //The functionality is in .cpp file (UserInterface.cpp)
    static void displayWelcomeMessage();
    static void getOriginAndDestination(std::string& origin, std::string& destination);
    static std::string getJourneyDate();
    static int getWaitingListNumber();

    static void displayTrains(const std::vector<Train>& trains, const std::string& type,
                              const std::map<std::string, double>& probabilities);
    static void displayTravelPlans(const std::vector<TravelPlan>& plans);
    static void displayErrorMessage(const std::string& message);
};

#endif