#include "json.hpp"
#include "tree.hpp"
#include <iostream>

int main() {
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
    for (const auto &treeJson : treeJsons) {
        // Create a tree from JSON
        Tree tree = createTreeFromJson(treeJson);
        // Output tree properties
        std::cout << "Tree ID: " << tree.getId() << std::endl;
        std::cout << "Latitude: " << tree.getLat() << std::endl;
        std::cout << "Longitude: " << tree.getLon() << std::endl;
        std::cout << "Genus: " << tree.getGenus() << std::endl;
        std::cout << "Species: " << tree.getSpecies() << std::endl;
        std::cout << "Height: " << tree.getHeight() << std::endl;
        std::cout << "Circumference: " << tree.getCircumference() << std::endl;
        std::cout << "Crown Diameter: " << tree.getDiameterCrown() << std::endl;

        return 0;
    }
}