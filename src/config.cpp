#include "../include/config.hpp"
#include "../include/json.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

Config::Config(std::string const &filename) {
    std::ifstream ifile(filename);
    if (ifile) {
        nlohmann::json data = nlohmann::json::parse(ifile);
        M_bbox = data["bbox"];
        M_LOD = data["LOD"];
        M_output_name = data["output_name"];
        M_default_genus = data["default_genus"];
        M_default_height = data["default_height"];
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

Config::~Config() {}

std::ostream &operator<<(std::ostream &os, const Config &config) {
    os << std::setprecision(10) << "Bounding box: " << config.bbox()
       << std::endl;
    os << "LOD: " << config.LOD() << std::endl;
    return os;
}