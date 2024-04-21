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

double treeHeightKNN(double dist, Tree &t,
                     std::vector<Tree> const &treeLibrary) {
    std::vector<Tree> knn;
    for (auto &tree : treeLibrary) {
        if (t.id() != tree.id()) {
            double d =
                sqrt(pow(t.x() - tree.x(), 2) + pow(t.y() - tree.y(), 2));
            if (d <= dist) {
                knn.push_back(tree);
            }
        }
    }

    double sum = 0;
    int count = 0;
    for (auto &tree : knn) {
        if (tree.height() != 0) {
            sum += tree.height();
            ++count;
        }
    }

    if (count == 0)
        return 0;

    return sum / count;
}
