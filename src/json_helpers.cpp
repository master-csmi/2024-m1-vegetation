#include "../include/json_helpers.hpp"
#include "../include/json.hpp"
#include "../include/tree.hpp"

std::vector<Tree> createLibraryFromJson(nlohmann::json const &jsonData) {
    std::vector<Tree> treeLibrary;
    for (auto &element : jsonData["elements"]) {
        treeLibrary.push_back(createTreeFromJson(element));
    }

    return treeLibrary;
}

double treeHeightKNN(int k, Tree &t, std::vector<Tree> const &treeLibrary) {
    // Compute the distance to all other trees and store them along with their
    // heights
    std::vector<std::pair<double, double>> distances;
    for (auto &tree : treeLibrary) {
        if (t.id() != tree.id() && tree.height() != 0) {
            double d =
                sqrt(pow(t.x() - tree.x(), 2) + pow(t.y() - tree.y(), 2));
            distances.push_back({d, tree.height()});
        }
    }

    // Sort by distance
    std::sort(distances.begin(), distances.end());

    // Check if we have at least k trees
    if (distances.size() < k) {
        std::cerr << "Not enough trees with non-zero height." << std::endl;
        return -1;
    }

    // Compute the average height of the k nearest trees
    double sum = 0;
    for (int i = 0; i < k; i++) {
        sum += distances[i].second;
    }

    return sum / k;
}
