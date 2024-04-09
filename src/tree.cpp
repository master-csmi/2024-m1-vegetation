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
        tree.id = treeJson["id"].get<int>();
    }

    if (!treeJson["lat"].is_null())
    {
        tree.lat = treeJson["lat"].get<double>();
    }

    if (!treeJson["lon"].is_null())
    {
        tree.lon = treeJson["lon"].get<double>();
    }

    if (!treeJson["genus"].is_null())
    {
        tree.genus = treeJson["genus"].get<std::string>();
    }

    if (!treeJson["species"].is_null())
    {
        tree.species = treeJson["species"].get<std::string>();
    }

    if (!treeJson["height"].is_null())
    {
        tree.height = treeJson["height"].get<double>();
    }

    if (!treeJson["circumference"].is_null())
    {
        tree.circumference = treeJson["circumference"].get<double>();
    }

    if (!treeJson["diameter_crown"].is_null())
    {
        tree.diameter_crown = treeJson["diameter_crown"].get<double>();
    }

    return tree;
}