#ifndef QUERY_HPP
#define QUERY_HPP

#include "json.hpp"
#include <string>

void perform_query(std::string bbox);
nlohmann::json get_query_result();

#endif