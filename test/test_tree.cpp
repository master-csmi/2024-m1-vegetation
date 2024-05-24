#include "../include/json.hpp"
#include "../include/tree.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Tree class", "[tree]") {
    // Sample JSON data representing a tree
    std::vector<nlohmann::json> jsonData = {
        {{"id", 1},
         {"lat", 37.7749},
         {"lon", -122.4194},
         {"genus", "Quercus"},
         {"species", "rubra"},
         {"height", 20.5},
         {"circumference", 120.0},
         {"diameter_crown", 8.0}},
        {
            {"id", 2},
            {"lat", 37.7749},
            {"lon", -122.4194},
            {"genus", "Quercus"},
            {"species", "rubra"},
            {"height", 20.5},
            {"circumference", 120.0},
        },
    };

    // Create Tree objects from the JSON data
    auto treeLibrary = createLibraryFromJson(jsonData);

    // Check the number of trees created
    REQUIRE(treeLibrary.size() == 2);

    // Check the attributes of the first tree
    REQUIRE(treeLibrary[0].id() == 1);
    REQUIRE(treeLibrary[0].lat() == 37.7749);
    REQUIRE(treeLibrary[0].lon() == -122.4194);
    REQUIRE(treeLibrary[0].genus() == "Quercus");
    REQUIRE(treeLibrary[0].species() == "rubra");
    REQUIRE(treeLibrary[0].height() == 20.5);
    REQUIRE(treeLibrary[0].circumference() == 120.0);
    REQUIRE(treeLibrary[0].diameterCrown() == 8.0);

    // Check the attributes of the second tree
    REQUIRE(treeLibrary[1].id() == 2);
    REQUIRE(treeLibrary[1].lat() == 37.7749);
    REQUIRE(treeLibrary[1].lon() == -122.4194);
    REQUIRE(treeLibrary[1].genus() == "Quercus");
    REQUIRE(treeLibrary[1].species() == "rubra");
    REQUIRE(treeLibrary[1].height() == 20.5);
    REQUIRE(treeLibrary[1].circumference() == 120.0);
    REQUIRE(treeLibrary[1].diameterCrown() == 0.0);

    // Check the computed x and y coordinates of the first tree
    treeLibrary[0].computeXY(37.7749, -122.4194);
    REQUIRE(treeLibrary[0].x() == 0.0);
    REQUIRE(treeLibrary[0].y() == 0.0);

    // Check the computed x and y coordinates of the second tree
    treeLibrary[1].computeXY(37.7749, -122.4194);
    REQUIRE(treeLibrary[1].x() == 0.0);

    // Check the wrap method
    treeLibrary[0].wrap(0);
}