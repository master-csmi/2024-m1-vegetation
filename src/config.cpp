#include "../include/config.hpp"
#include "../include/json.hpp"
#include <fstream>
#include <iomanip>
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
        M_output_name = data["output_name"];
        M_default_genus = data["default_genus"];
        M_default_height = data["default_height"];
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

Config::~Config() {}

double Config::Alat() const { return A_lat; }
double Config::Alon() const { return A_lon; }
double Config::Blat() const { return B_lat; }
double Config::Blon() const { return B_lon; }
double Config::LOD() const { return M_LOD; }
Query Config::query() const { return Query(A_lat, A_lon, B_lat, B_lon); }
std::string Config::output_name() const { return M_output_name; }

std::ostream &operator<<(std::ostream &os, const Config &config) {
    os << std::setprecision(10) << "A lat: " << config.Alat()
       << ", A lon: " << config.Alon() << std::endl;
    os << std::setprecision(10) << "B lat: " << config.Blat()
       << ", B lon: " << config.Blon() << std::endl;
    os << "LOD: " << config.LOD() << std::endl;
    return os;
}