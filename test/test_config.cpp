#include "../include/config.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cmath>

#define EPSILON 1e-6

TEST_CASE("Config class", "[config]") {
    Config config("../../test/test_config.json");
    REQUIRE(fabs(config.bbox_coords()[0] - 48.58900) < EPSILON);
    REQUIRE(fabs(config.bbox_coords()[1] - 7.75551) < EPSILON);
    REQUIRE(fabs(config.bbox_coords()[2] - 48.59193) < EPSILON);
    REQUIRE(fabs(config.bbox_coords()[3] - 7.76066) < EPSILON);

    REQUIRE(config.origin() == "48.583055227464364, 7.748664426560083");
    REQUIRE(config.LOD() == 0);
    REQUIRE(config.altitude() == 0);
    REQUIRE(config.default_height() == "3, 40");
    REQUIRE(config.default_genus() == "Platanus");
    REQUIRE(config.input_building_mesh() == "mesh_lod1.stl");
    REQUIRE(config.output_name() == "contades");

    auto bbox_coords = config.bbox_coords();
    REQUIRE(fabs(bbox_coords[0] - 48.58900) < EPSILON);
    REQUIRE(fabs(bbox_coords[1] - 7.75551) < EPSILON);
    REQUIRE(fabs(bbox_coords[2] - 48.59193) < EPSILON);
    REQUIRE(fabs(bbox_coords[3] - 7.76066) < EPSILON);

    std::pair<double, double> origin = string_to_pair(config.origin());
    REQUIRE(fabs(origin.first - 48.583055227464364) < EPSILON);
    REQUIRE(fabs(origin.second - 7.748664426560083) < EPSILON);
}
