#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

// Define a Tree class
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

    Tree(long id, double lat, double lon, std::string genus, std::string species, double height, double circumference, double diameter_crown)
        : id(id), lat(lat), lon(lon), genus(genus), species(species), height(height), circumference(circumference), diameter_crown(diameter_crown) {}
};

// Function to create a Tree object from a JSON object
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

// Function to create a vector of Tree objects from a JSON file
std::vector<Tree> createLibraryFromJsonFile(std::string filename)
{
    std::ifstream inputFile(filename);
    nlohmann::json jsonData;
    inputFile >> jsonData;

    std::vector<Tree> treeLibrary;
    for (auto &element : jsonData["elements"])
    {
        treeLibrary.push_back(createTreeFromJson(element));
    }

    return treeLibrary;
}

int main()
{
    std::vector<Tree> treeLibrary = createLibraryFromJsonFile("query_result.json");

    // Now you can use the treeLibrary vector to access your tree data
    for (const auto &tree : treeLibrary)
    {
        std::cout << "Tree ID: " << tree.id << " Lat, Lon: (" << tree.lat
                  << ", " << tree.lon << "), Genus: " << tree.genus << ", Species: "
                  << tree.species << ", Height: " << tree.height << ", Circumference: "
                  << tree.circumference << ", Diameter_crown: " << tree.diameter_crown
                  << std::endl;
    }

    return 0;
}