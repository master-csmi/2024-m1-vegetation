#include "tree.hpp"

Tree::Tree(long id, double lat, double lon, std::string genus, std::string species, double height, double circumference, double diameter_crown)
    : id(id), lat(lat), lon(lon), genus(genus), species(species), height(height), circumference(circumference), diameter_crown(diameter_crown) {}

Tree createTreeFromJson(nlohmann::json treeJson)
{
    long id = treeJson["id"];
    double lat = treeJson["lat"];
    double lon = treeJson["lon"];
    std::string genus = treeJson["tags"]["genus"];
    std::string species = treeJson["tags"]["species"];

    double height = 0.0, circumference = 0.0, diameter_crown = 0.0;
    if (treeJson["tags"]["height"].is_string())
        height = std::stod(treeJson["tags"]["height"].get<std::string>());
    if (treeJson["tags"]["circumference"].is_string())
        circumference = std::stod(treeJson["tags"]["circumference"].get<std::string>());
    if (treeJson["tags"]["diameter_crown"].is_string())
        diameter_crown = std::stod(treeJson["tags"]["diameter_crown"].get<std::string>());

    return Tree(id, lat, lon, genus, species, height, circumference, diameter_crown);
}