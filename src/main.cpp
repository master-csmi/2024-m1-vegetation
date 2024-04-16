#include "../include/config.hpp"
#include "../include/json_helpers.hpp"
#include "../include/mesh.hpp"
#include "../include/query.hpp"
#include "../include/tree.hpp"
// #include <cpr/cpr.h>
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

    query.perform_query();
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

    for (auto &tree : treeLibrary) {
        tree.computeXY(query);
        // std::cout << "Tree X: " << tree.getX() << std::endl;
        // std::cout << "Tree Y: " << tree.getY() << std::endl;

        TreeMesh mesh;
        mesh.setLod(0);
        mesh.wrap_tree(tree);
        tree.setMesh(mesh);
        mesh.dumpMesh(tree);
    }

    return 0;
}