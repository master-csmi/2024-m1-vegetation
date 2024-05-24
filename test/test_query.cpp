#include "../include/query.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("perform query function", "[query]") {
    perform_query("48.58900,7.75551,48.59193,7.76066"); // contades
    nlohmann::json jsonData = get_query_result();
    REQUIRE(jsonData["version"] == 0.6);
    REQUIRE(jsonData["generator"] == "Overpass API 0.7.62.1 084b4234");
    REQUIRE(jsonData["elements"].size() > 10);
}