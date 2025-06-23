#ifndef STATION_H
#define STATION_H

#include <string>

struct Station {
    std::string code;
    std::string name;

    Station(const std::string& c = "", const std::string& n = "") : code(c), name(n) {}
};

#endif // STATION_H