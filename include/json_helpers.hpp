#ifndef JSON_HELPERS_HPP
#define JSON_HELPERS_HPP

#include "json.hpp"
#include <vector>

class Tree;

std::vector<Tree> createLibraryFromJson(nlohmann::json const &jsonData);
double treeHeightKNN(int k, Tree &t, std::vector<Tree> const &treeLibrary);

#endif