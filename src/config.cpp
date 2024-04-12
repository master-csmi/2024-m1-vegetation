#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "config.hpp"

// Constructor definition
Config::Config(std::string const& filename)
{
    std::ifstream ifile(filename);
    if (ifile)
    {
        nlohmann::json data = nlohmann::json::parse(ifile);
        A_lat = data["A"]["latitude"];
        A_lon = data["A"]["longitude"];
        B_lat = data["B"]["latitude"];
        B_lon = data["B"]["longitude"];
    }
    else
    {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

// Destructor definition
Config::~Config() {}

// Getters definition
double Config::getAlat() const { return A_lat; }

double Config::getAlon() const { return A_lon; }

double Config::getBlat() const { return B_lat; }

double Config::getBlon() const { return B_lon; }

// Get the query
Query Config::get_query() const
{
    return Query(A_lat, A_lon, B_lat, B_lon);
}