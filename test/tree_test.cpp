#include <iostream>
#include "json_helpers.hpp"

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