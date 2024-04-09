#include "tree.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

int main()
{
    // Test the Tree class and createTreeFromJson function

    // Sample JSON data representing a tree
    std::vector<nlohmann::json> treeJsons = {
        {{"id", 1},
         {"lat", 37.7749},
         {"lon", -122.4194},
         {"genus", "Quercus"},
         {"species", "rubra"},
         {"height", 20.5},
         {"circumference", 120.0},
         {"diameter_crown", 8.0}},
        {
            {"id", 2},
            {"lat", 37.7749},
            {"lon", -122.4194},
            {"genus", "Quercus"},
            {"species", "rubra"},
            {"height", 20.5},
            {"circumference", 120.0},
            // "diameter_crown" field is missing
        },
    };

    // Test each JSON object
    for (const auto &treeJson : treeJsons)
    {
        // Create a tree from JSON
        Tree tree = createTreeFromJson(treeJson);

        // Output tree properties
        std::cout << "Tree ID: " << tree.id << std::endl;
        std::cout << "Latitude: " << tree.lat << std::endl;
        std::cout << "Longitude: " << tree.lon << std::endl;
        std::cout << "Genus: " << tree.genus << std::endl;
        std::cout << "Species: " << tree.species << std::endl;
        std::cout << "Height: " << tree.height << std::endl;
        std::cout << "Circumference: " << tree.circumference << std::endl;
        std::cout << "Crown Diameter: " << tree.diameter_crown << std::endl;

        return 0;
    }
}