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
    std::string bbox = config.bbox();
    double ref_lat = config.bbox_coords()[0];
    double ref_lon = config.bbox_coords()[1];
    std::map<std::string, GenusHeight> genusHeights;
    std::array<double, 2> B_cart;
    double Bx, By, area, height;
    nlohmann::json jsonData;
    std::map<std::string, double> defaultHeights;
    std::string genus;

    std::cout << config << std::endl;

    // Compute the area of the bounding box
    B_cart = wgs84::toCartesian(
        {ref_lat, ref_lon} /* reference position */,
        {config.bbox_coords()[2],
         config.bbox_coords()[3]} /* position to be converted */);

    Bx = B_cart[0];
    By = B_cart[1];
    area = Bx * By; // A is (0, 0) : origin

    // Perform the Overpass query
    perform_query(bbox);
    jsonData = get_query_result();

    // Compute average height for each genus
    for (const auto &tree : jsonData["elements"]) {
        if (tree.contains("tags") && tree["tags"].contains("genus")) {
            genus = tree["tags"]["genus"].get<std::string>();
            height = 0;
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
    for (const auto &entry : genusHeights)
        defaultHeights[entry.first] = entry.second.averageHeight();

    // Generate tree objects from the JSON data
    auto treeLibrary = createLibraryFromJson(jsonData);

    // Mesh part
    Mesh finalMesh;
    Mesh currentWrap;
    std::string output_name = config.output_name();
    std::string output_folder, filename, metrics_filename;
    int lod = config.LOD();
    CGAL::Real_timer t;
    double defaultHeight = config.default_height();
    int nNoHeight = 0;
    int nNoGenus = 0;
    double h;

    std::cout << "Computing the union of tree meshes ..." << std::endl;
    t.start();
    for (auto &tree : treeLibrary) {
        tree.computeXY(ref_lat, ref_lon);

        // std::cout << tree << std::endl;

        if (tree.genus() == "") {
            ++nNoGenus;
            tree.setGenus(config.default_genus());
        }

        if (tree.height() == 0) {
            ++nNoHeight;
            if (defaultHeights.find(tree.genus()) != defaultHeights.end()) {
                h = defaultHeights[tree.genus()];
            } else {
                h = defaultHeight;
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

    // Final mesh export
    output_folder = "../output/";
    if (!std::filesystem::exists(output_folder)) {
        std::filesystem::create_directory(output_folder);
    }
    filename =
        "../output/" + output_name + "_LOD" + std::to_string(lod) + ".stl";

    std::ofstream output(filename);
    CGAL::IO::write_STL(output, finalMesh);

    std::cout << "Final mesh: " << num_vertices(finalMesh) << " vertices, "
              << num_faces(finalMesh) << " faces" << std::endl;
    std::cout << "Took " << t.time() << " s." << std::endl;
    std::cout << "Final mesh written to " << filename << std::endl;

    // Metrics export
    metrics_filename = "../output/" + output_name + "_metrics_LOD" +
                       std::to_string(lod) + ".txt";

    std::ofstream metrics(metrics_filename);
    metrics << "Area: " << area << " meters" << std::endl;
    metrics << "Total number of trees: " << treeLibrary.size() << std::endl;
    metrics << "Number of tree which had no height: " << nNoHeight << std::endl;
    metrics << "Number of tree which had no genus: " << nNoGenus << std::endl;
    metrics << "Number of vertices: " << num_vertices(finalMesh) << std::endl;
    metrics << "Number of faces: " << num_faces(finalMesh) << std::endl;
    metrics << "Time to mesh: " << t.time() << " seconds" << std::endl;
    metrics.close();

    std::cout << "Metrics available at " << metrics_filename << std::endl;

    exit(0);
}
