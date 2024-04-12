#ifndef JSON_HELPERS_HPP
#define JSON_HELPERS_HPP

#include <string>
#include <vector>
#include "tree.hpp"

std::vector<Tree> createLibraryFromJson(nlohmann::json const &jsonData);

#endif