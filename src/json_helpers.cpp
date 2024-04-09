#include "json_helpers.hpp"
#include <fstream>
#include "tree.hpp"
#include <nlohmann/json.hpp>

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