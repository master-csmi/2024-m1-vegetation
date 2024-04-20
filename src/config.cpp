#include "../include/config.hpp"
#include "../include/json.hpp"
#include <fstream>
#include <iostream>

Config::Config(std::string const &filename) {
    std::ifstream ifile(filename);
    if (ifile) {
        nlohmann::json data = nlohmann::json::parse(ifile);
        A_lat = data["A"]["latitude"];
        A_lon = data["A"]["longitude"];
        B_lat = data["B"]["latitude"];
        B_lon = data["B"]["longitude"];
        M_LOD = data["LOD"];
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

Config::~Config() {}

double Config::getAlat() const { return A_lat; }

double Config::getAlon() const { return A_lon; }

double Config::getBlat() const { return B_lat; }

double Config::getBlon() const { return B_lon; }

double Config::getLOD() const { return M_LOD; }

Query Config::get_query() const { return Query(A_lat, A_lon, B_lat, B_lon); }

std::ostream &operator<<(std::ostream &os, const Config &config) {
    os << "A lat: " << config.A_lat << ", A lon: " << config.A_lon << std::endl;
    os << "B lat: " << config.B_lat << ", B lon: " << config.B_lon << std::endl;
    os << "LOD: " << config.M_LOD << std::endl;
    return os;
}