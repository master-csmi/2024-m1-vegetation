#include "../include/WGS84toCartesian.hpp"
#include "../include/config.hpp"
#include "../include/default_tree.hpp"
#include "../include/json_helpers.hpp"
#include "../include/query.hpp"
#include "../include/tree.hpp"
// #include <cpr/cpr.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/Color.h>
#include <CGAL/IO/STL.h>
#include <CGAL/IO/polygon_soup_io.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
// #include <CGAL/Polygon_mesh_processing/autorefinement.h>
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
#include <filesystem>
#include <iostream>
#include <string>

#include <boost/container/small_vector.hpp>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point_3 = K::Point_3;
using Mesh = CGAL::Surface_mesh<Point_3>;
namespace PMP = CGAL::Polygon_mesh_processing;

void clean_mesh(Mesh &mesh) {
    PMP::duplicate_non_manifold_vertices(mesh);
    PMP::remove_degenerate_faces(mesh);
    PMP::remove_isolated_vertices(mesh);
    PMP::stitch_borders(mesh);
}

int main(int argc, char **argv) {

    Config config("config.json");
    std::string bbox = config.bbox();

    std::map<std::string, GenusHeight> genusHeights;
    std::array<double, 2> B_cart;
    double Bx, By, area, height;
    nlohmann::json jsonData;
    std::map<std::string, double> defaultHeights;
    std::string genus;

    std::cout << config << std::endl;

    // Compute the area of the bounding box
    B_cart = wgs84::toCartesian(
        {config.bbox_coords()[0],
         config.bbox_coords()[1]} /* reference position */,
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
    Mesh finalMesh, currentWrap, building_mesh;
    std::string output_name = config.output_name();
    std::string height_range_str = config.default_height();
    std::string output_folder, filename, metrics_filename;
    int lod = config.LOD();
    CGAL::Real_timer t;
    int nNoHeight = 0;
    int nNoGenus = 0;
    double h;
    std::string origin = config.origin();
    std::pair<double, double> origin_double = string_to_pair(origin);
    std::pair<double, double> heigth_range = string_to_pair(height_range_str);
    const std::string building_mesh_str =
        CGAL::data_file_path(config.input_building_mesh());
    std::vector<Point_3> input_points;
    std::vector<boost::container::small_vector<std::size_t, 3>> input_triangles;

    std::cout << "Computing the union of tree meshes ..." << std::endl;
    t.start();
    for (auto &tree : treeLibrary) {
        tree.load_data("trees.json");
        tree.computeXY(origin_double.first, origin_double.second);

        // std::cout << tree << std::endl;

        if (tree.genus() == "") {
            ++nNoGenus;
            tree.setGenus(config.default_genus());
        }

        if (tree.height() == 0) {
            ++nNoHeight;
            h = heigth_range.first +
                static_cast<double>(rand()) /
                    (static_cast<double>(
                        RAND_MAX / (heigth_range.second - heigth_range.first)));

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

    // if (!CGAL::IO::read_polygon_soup(building_mesh_str, input_points,
    //                                  input_triangles)) {
    //     std::cerr << "Cannot read " << building_mesh_str << "\n";
    //     return 1;
    // }
    // PMP::repair_polygon_soup(input_points, input_triangles);
    // PMP::triangulate_polygons(input_points, input_triangles);

    // PMP::autorefine_triangle_soup(
    //     input_points, input_triangles,
    //     CGAL::parameters::concurrency_tag(CGAL::Parallel_if_available_tag()));

    // std::cout << "Computing the union of building and tree meshes ..."
    //           << std::endl;
    // if (!CGAL::Polygon_mesh_processing::corefine_and_compute_union(
    //         finalMesh, building_mesh, finalMesh)) {
    //     std::cerr << "Corefine_and_compute_union buildings and trees failed"
    //               << std::endl;
    //     exit(1);
    // }

    // std::cout << "Computing the union of building and tree meshes ..."
    //           << std::endl;
    // if (!CGAL::Polygon_mesh_processing::corefine_and_compute_union(
    //         finalMesh, building_mesh, finalMesh)) {
    //     std::cerr << "Corefine_and_compute_union buildings and trees failed"
    //               << std::endl;
    //     exit(1);
    // }

    t.stop();

    // Final mesh export
    output_folder = "output/";
    if (!std::filesystem::exists(output_folder)) {
        std::filesystem::create_directory(output_folder);
    }
    filename =
        output_folder + output_name + "_lod" + std::to_string(lod) + ".stl";

    std::ofstream output(filename);
    CGAL::IO::write_STL(output, finalMesh);

    std::cout << "" << std::endl;
    std::cout << "Final mesh: " << num_vertices(finalMesh) << " vertices, "
              << num_faces(finalMesh) << " faces" << std::endl;
    std::cout << "Took " << t.time() << " s. (" << t.time() / 60.0
              << " minutes)" << std::endl;
    std::cout << "Final mesh written to " << filename << std::endl;

    // Metrics export
    metrics_filename = output_folder + output_name + "_metrics_LOD" +
                       std::to_string(lod) + ".txt";

    std::ofstream metrics(metrics_filename);
    metrics << "Area: " << std::sqrt(area) << " squared meters" << std::endl;
    metrics << "Total number of trees: " << treeLibrary.size() << std::endl;
    metrics << "Number of tree which had no height: " << nNoHeight << std::endl;
    metrics << "Number of tree which had no genus: " << nNoGenus << std::endl;
    metrics << "Number of vertices: " << num_vertices(finalMesh) << std::endl;
    metrics << "Number of faces: " << num_faces(finalMesh) << std::endl;
    metrics << "Time to mesh: " << t.time() << " seconds, (" << t.time() / 60.0
            << " minutes)" << std::endl;
    metrics.close();

    std::cout << "Metrics available at " << metrics_filename << std::endl;

    exit(0);
}
