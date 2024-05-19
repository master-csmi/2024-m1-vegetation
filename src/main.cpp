#include "../include/WGS84toCartesian.hpp"
#include "../include/config.hpp"
#include "../include/default_tree.hpp"
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
    double ref_lat = config.Alat();
    double ref_lon = config.Alon();
    double delta = 0.5;

    std::cout << config << std::endl;

    std::array<double, 2> B_cart = wgs84::toCartesian(
        {ref_lat, ref_lon} /* reference position */,
        {config.Blat(), config.Blon()} /* position to be converted */);

    double Bx = B_cart[0];
    double By = B_cart[1];
    double area = Bx * By; // A is (0, 0) : origin

    // Increase the bounding box
    if (area < 100'000) {
        config.setAlat(config.Alat() + delta);
        config.setAlon(config.Alon() - delta);
        config.setBlat(config.Blat() - delta);
        config.setBlon(config.Blon() + delta);
    }

    auto query = config.query();

    query.perform_query();

    nlohmann::json jsonData = query.get_query_result();

    // Compute average height for each genus
    std::map<std::string, GenusHeight> genusHeights;

    for (const auto &tree : jsonData["elements"]) {
        if (tree.contains("tags") && tree["tags"].contains("genus")) {
            std::string genus = tree["tags"]["genus"].get<std::string>();
            double height = 0;
            if (tree["tags"].contains("height")) {
                try {
                    height =
                        std::stod(tree["tags"]["height"].get<std::string>());
                } catch (const std::exception &e) {
                    std::cerr
                        << "Error converting height to double: " << e.what()
                        << std::endl;
                    continue;
                }
            }

            if (genusHeights.find(genus) == genusHeights.end()) {
                genusHeights[genus] = GenusHeight();
                genusHeights[genus].genus = genus;
            }

            genusHeights[genus].addHeight(height);
        }
    }

    std::map<std::string, double> defaultHeights;
    for (const auto &entry : genusHeights) {
        defaultHeights[entry.first] = entry.second.averageHeight();
    }

    if (area < 100'000) {
        // Reset bounding box to original values and recompute query
        config.setAlat(config.Alat() - delta);
        config.setAlon(config.Alon() + delta);
        config.setBlat(config.Blat() + delta);
        config.setBlon(config.Blon() - delta);

        query = config.query();
        query.perform_query();
        jsonData = query.get_query_result();
    }

    // Generate tree objects from the JSON data
    auto treeLibrary = createLibraryFromJson(jsonData);

    // Mesh part
    Mesh finalMesh;
    Mesh currentWrap;
    std::string output_name = config.output_name();
    int lod = config.LOD();
    CGAL::Real_timer t;
    double defaultHeight = config.default_height();
    int nNoHeight = 0;
    int nNoGenus = 0;

    std::cout << "Computing the union of tree meshes ..." << std::endl;
    t.start();
    for (auto &tree : treeLibrary) {
        tree.computeXY(ref_lat, ref_lon);

        // std::cout << tree << std::endl;

        if (tree.height() == 0) {
            ++nNoHeight;
            double h;
            if (tree.genus() == "") {
                ++nNoGenus;
                h = defaultHeight;
            } else {
                h = defaultHeights[tree.genus()];
            }

            tree.setHeight(h);
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

    // Metrics

    std::string metrics_filename = "../output/" + output_name + "_metrics_LOD" +
                                   std::to_string(lod) + ".txt";

    std::ofstream metrics(metrics_filename);
    metrics << "Area: " << area << " m^2" << std::endl;
    metrics << "Total number of trees: " << treeLibrary.size() << std::endl;
    metrics << "Number of tree which had no height: " << nNoHeight << std::endl;
    metrics << "Number of tree which had no genus: " << nNoGenus << std::endl;
    metrics << "Number of vertices: " << num_vertices(finalMesh) << std::endl;
    metrics << "Number of faces: " << num_faces(finalMesh) << std::endl;
    metrics << "Time: " << t.time() << " s." << std::endl;
    metrics.close();

    exit(0);
}
