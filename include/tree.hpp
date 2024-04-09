#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include <nlohmann/json.hpp>

class Tree
{
public:
    long id;
    double lat;
    double lon;
    std::string genus;
    std::string species;
    double height;
    double circumference;
    double diameter_crown;

    Tree(long id, double lat, double lon, std::string genus, std::string species, double height, double circumference, double diameter_crown);
};

Tree createTreeFromJson(nlohmann::json treeJson);

#endif