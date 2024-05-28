#ifndef JSON_HELPERS_HPP
#define JSON_HELPERS_HPP

#include "json.hpp"
#include <vector>

class Tree;

/**
 * @brief Create a Library of Tree From Json object
 *
 * @param jsonData a nlohmann::json object
 * @return std::vector<Tree> a vector of Tree
 */
std::vector<Tree> createLibraryFromJson(nlohmann::json const &jsonData);

#endif