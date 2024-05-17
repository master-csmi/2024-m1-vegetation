#include "../include/config.hpp"
#include "../include/json_helpers.hpp"
#include "../include/query.hpp"
#include "../include/tree.hpp"
// #include <cpr/cpr.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Real_timer.h>
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char **argv) {

    Config config("../config.json");
    bool do_query = config.new_query();

    std::cout << config << std::endl;

    auto query = config.query();

    if (do_query)
        query.perform_query();

    nlohmann::json jsonData = query.get_query_result();

    // Generate tree objects from the JSON data
    auto treeLibrary = createLibraryFromJson(jsonData);

    // for (auto &tree : treeLibrary) {
    //     std::cout << tree << std::endl;
    // }

    // Mesh part
    Mesh finalMesh;
    Mesh currentWrap;
    double ref_lat = config.Alat();
    double ref_lon = config.Alon();
    std::string output_name = config.output_name();
    int lod = config.LOD();
    CGAL::Real_timer t;
    double distKNN = config.distKNN();

    std::cout << "Computing the union of the meshes..." << std::endl;
    t.start();
    for (auto &tree : treeLibrary) {
        tree.computeXY(ref_lat, ref_lon);

        if (tree.height() == 0) {
            std::cout << "Tree " << tree.id()
                      << " has no height, computing KNN with distance "
                      << distKNN;
            tree.setHeight(treeHeightKNN(distKNN, tree, treeLibrary));
            std::cout << ", tree.height=" << tree.height() << std::endl;
        }

        tree.wrap(lod);
        currentWrap = tree.wrap();

        // Compute the union of the meshes
        if (!CGAL::Polygon_mesh_processing::corefine_and_compute_union(
                finalMesh, currentWrap, finalMesh)) {
            std::cerr << "Corefine_and_compute_union failed." << std::endl;
            exit(1);
        }
    }
    t.stop();

    std::string output_folder = "../output/";
    if (!std::filesystem::exists(output_folder)) {
        std::filesystem::create_directory(output_folder);
    }
    std::string filename =
        "../output/" + output_name + "_LOD" + std::to_string(lod) + ".stl";

    std::ofstream output(filename);
    CGAL::IO::write_STL(output, finalMesh);

    std::cout << "Final mesh: " << num_vertices(finalMesh) << " vertices, "
              << num_faces(finalMesh) << " faces" << std::endl;
    std::cout << "Took " << t.time() << " s." << std::endl;
    std::cout << "Final mesh written to " << filename << std::endl;

    exit(0);
}
