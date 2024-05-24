#include "../include/json.hpp"
#include "../include/json_helpers.hpp"
#include "../include/query.hpp"
#include "../include/tree.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>

#define EPSILON 1e-6

TEST_CASE("Tree class", "[tree]") {
    perform_query("48.58715,7.75364,48.58731,7.75438");

    nlohmann::json jsonData = get_query_result();

    // Create Tree objects from the JSON data
    auto treeLibrary = createLibraryFromJson(jsonData);

    // Check the attributes of the first tree
    REQUIRE(treeLibrary[0].id() == 1572084575);
    REQUIRE(treeLibrary[0].lat() == 48.5872656);
    REQUIRE(treeLibrary[0].lon() == 7.7540658);
    REQUIRE(treeLibrary[0].genus() == "Taxus");
    REQUIRE(treeLibrary[0].species() == "Taxus baccata");
    REQUIRE(treeLibrary[0].height() == 4);
    REQUIRE(treeLibrary[0].circumference() == 1.5708);
    REQUIRE(treeLibrary[0].diameterCrown() == 4);

    // Check the attributes of the second tree
    REQUIRE(treeLibrary[1].id() == 1572084576);
    REQUIRE(treeLibrary[1].lat() == 48.5871975);
    REQUIRE(treeLibrary[1].lon() == 7.7538365);
    REQUIRE(treeLibrary[1].genus() == "Taxus");
    REQUIRE(treeLibrary[1].species() == "Taxus baccata");
    REQUIRE(treeLibrary[1].height() == 4);
    REQUIRE(treeLibrary[1].circumference() == 0.91106);
    REQUIRE(treeLibrary[1].diameterCrown() == 3);

    // Check the computed x and y coordinates of the first tree
    treeLibrary[0].computeXY(48.5915821, 7.7566818);
    REQUIRE(fabs(treeLibrary[0].x() + 192.9940319197) < EPSILON);
    REQUIRE(fabs(treeLibrary[0].y() + 479.9991617497) < EPSILON);

    // Check the computed x and y coordinates of the second tree
    treeLibrary[1].computeXY(48.5871975, 7.7538365);
    REQUIRE(treeLibrary[1].x() == 0.0);
    REQUIRE(treeLibrary[1].y() == 0.0);

    // Check the wrap method
    treeLibrary[0].wrap(0, "../../");
}