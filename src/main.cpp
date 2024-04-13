#include "config.hpp"
#include "json_helpers.hpp"
#include "query.hpp"
#include "tree.hpp"
#include <cpr/cpr.h>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    Config config("../config.json");

    std::cout << "A_lat: " << config.getAlat() << std::endl;
    std::cout << "A_lon: " << config.getAlon() << std::endl;
    std::cout << "B_lat: " << config.getBlat() << std::endl;
    std::cout << "B_lon: " << config.getBlon() << std::endl;

    auto query = config.get_query();

    // query.perform_query();
    nlohmann::json jsonData = query.get_query_result();

    // Generate tree meshes
    auto treeLibrary = createLibraryFromJson(jsonData);

    for (auto &tree : treeLibrary) {
        std::cout << "Tree ID: " << tree.getId() << std::endl;
        std::cout << "Tree Lat: " << tree.getLat() << std::endl;
        std::cout << "Tree Lon: " << tree.getLon() << std::endl;
        std::cout << "Tree Genus: " << tree.getGenus() << std::endl;
        std::cout << "Tree Species: " << tree.getSpecies() << std::endl;
        std::cout << "Tree Height: " << tree.getHeight() << std::endl;
        std::cout << "Tree Circumference: " << tree.getCircumference()
                  << std::endl;
        std::cout << "Tree Diameter Crown: " << tree.getDiameterCrown()
                  << std::endl;
    }

    return 0;
}