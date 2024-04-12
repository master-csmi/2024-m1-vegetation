#include "tree.hpp"

// Default constructor
Tree::Tree() : id(0), lat(0), lon(0), height(0), circumference(0), diameter_crown(0) {}

// Constructor that takes arguments
Tree::Tree(long id, double lat, double lon, std::string genus, std::string species, double height, double circumference, double diameter_crown)
    : id(id), lat(lat), lon(lon), genus(genus), species(species), height(height), circumference(circumference), diameter_crown(diameter_crown) {}

Tree createTreeFromJson(const nlohmann::json &treeJson)
{
    Tree tree;

    if (!treeJson["id"].is_null())
    {
        tree.setId(treeJson["id"].get<int>());
    }

    if (!treeJson["lat"].is_null())
    {
        tree.setLat(treeJson["lat"].get<double>());
    }

    if (!treeJson["lon"].is_null())
    {
        tree.setLon(treeJson["lon"].get<double>());
    }

    if (!treeJson["tags"].is_null())
    {
        if (!treeJson["tags"]["genus"].is_null())
        {
            tree.setGenus(treeJson["tags"]["genus"].get<std::string>());
        }

        if (!treeJson["tags"]["species"].is_null())
        {
            tree.setSpecies(treeJson["tags"]["species"].get<std::string>());
        }

        if (!treeJson["tags"]["height"].is_null())
        {
            tree.setHeight(treeJson["tags"]["height"].get<double>());
        }

        if (!treeJson["tags"]["circumference"].is_null())
        {
            tree.setCircumference(treeJson["tags"]["circumference"].get<double>());
        }

        if (!treeJson["tags"]["diameter_crown"].is_null())
        {
            tree.setDiameterCrown(treeJson["tags"]["diameter_crown"].get<double>());
        }
    }

    return tree;
}