#include "../include/json.hpp"
#include "../include/json_helpers.hpp"
#include "../include/query.hpp"
#include "../include/tree.hpp"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/Color.h>
#include <CGAL/IO/STL.h>
#include <CGAL/IO/polygon_soup_io.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/fair.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup_extension.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/repair.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Real_timer.h>
#include <CGAL/Surface_mesh.h>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>

#include <boost/container/small_vector.hpp>

TEST_CASE("Union of 2 trees", "[union]") {
    perform_query("48.58715,7.75364,48.58731,7.75438");
    nlohmann::json jsonData = get_query_result();

    // Create Tree objects from the JSON data
    auto treeLibrary = createLibraryFromJson(jsonData);
    Mesh finalMesh, currentWrap;

    // Check the number of trees created
    REQUIRE(treeLibrary.size() == 2);

    for (auto &tree : treeLibrary) {
        tree.computeXY(48.5915821, 7.7566818);
        tree.wrap(0, "../../");
        currentWrap = tree.wrap();

        // Compute the union of the meshes
        if (!CGAL::Polygon_mesh_processing::corefine_and_compute_union(
                finalMesh, currentWrap, finalMesh)) {
            std::cerr << "Corefine_and_compute_union failed." << std::endl;
            exit(1);
        }
    }
    std::cout << "" << std::endl;
    std::cout << "Final mesh: " << num_vertices(finalMesh) << " vertices, "
              << num_faces(finalMesh) << " faces" << std::endl;
}