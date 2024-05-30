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
        M_altitude = data["altitude"];
        M_output_name = data["output_name"];
        M_default_genus = data["default_genus"];
        M_default_height = data["default_height_range"];
        M_origin = data["origin"];
        M_input_building_mesh = data["input_building_mesh"];
        M_merge = data["merge_buildings_trees"];
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

std::vector<double> Config::bbox_coords() const {
    std::vector<double> coords;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    std::string bbox = M_bbox; // Create a local copy of M_bbox.
    while ((pos = bbox.find(delimiter)) != std::string::npos) {
        token = bbox.substr(0, pos);
        coords.push_back(std::stod(token));
        bbox.erase(0, pos + delimiter.length());
    }
    coords.push_back(std::stod(bbox));
    return coords;
}

Config::~Config() {}

std::ostream &operator<<(std::ostream &os, const Config &config) {
    os << std::setprecision(10) << "Bounding box: " << config.bbox()
       << std::endl;
    os << "LOD: " << config.LOD() << std::endl;
    os << "Merging buildings and trees: " << std::boolalpha << config.merge()
       << std::endl;
    return os;
}

std::pair<double, double> string_to_pair(const std::string &origin) {
    double a, b;

    // Extract the longitude and latitude from the string
    size_t commaPos = origin.find(',');
    if (commaPos != std::string::npos) {
        std::string longStr = origin.substr(0, commaPos);
        std::string latStr = origin.substr(commaPos + 1);

        // Convert the longitude and latitude to double values
        try {
            a = std::stod(longStr);
            b = std::stod(latStr);
        } catch (const std::exception &e) {
            std::cerr << "Error converting coordinates to double: " << e.what()
                      << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Invalid coordinates format: " << origin << std::endl;
        exit(1);
    }

    return std::make_pair(a, b);
}