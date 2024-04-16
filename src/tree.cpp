#include "tree.hpp"
#include <string>

// Default constructor
Tree::Tree()
    : id(0), lat(0), lon(0), height(0), circumference(0), diameter_crown(0) {}

// Constructor that takes arguments
Tree::Tree(long id, double lat, double lon, std::string genus,
           std::string species, double height, double circumference,
           double diameter_crown)
    : id(id), lat(lat), lon(lon), genus(genus), species(species),
      height(height), circumference(circumference),
      diameter_crown(diameter_crown) {}

Tree createTreeFromJson(const nlohmann::json &treeJson) {
    Tree tree;

    if (treeJson.contains("id")) {
        tree.setId(treeJson["id"].get<int>());
    }

    if (treeJson.contains("lat")) {
        tree.setLat(treeJson["lat"].get<double>());
    }

    if (treeJson.contains("lon")) {
        tree.setLon(treeJson["lon"].get<double>());
    }

    if (treeJson.contains("tags")) {
        if (treeJson["tags"].contains("genus")) {
            tree.setGenus(treeJson["tags"]["genus"].get<std::string>());
        }

        if (treeJson["tags"].contains("species")) {
            tree.setSpecies(treeJson["tags"]["species"].get<std::string>());
        }

        if (treeJson["tags"].contains("height")) {
            tree.setHeight(
                std::stod(treeJson["tags"]["height"].get<std::string>()));
        }

        if (treeJson["tags"].contains("circumference")) {
            tree.setCircumference(std::stod(
                treeJson["tags"]["circumference"].get<std::string>()));
        }

        if (treeJson["tags"].contains("diameter_crown")) {
            tree.setDiameterCrown(std::stod(
                treeJson["tags"]["diameter_crown"].get<std::string>()));
        }
    }

    return tree;
}