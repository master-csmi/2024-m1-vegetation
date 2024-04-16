#ifndef MESH_HPP
#define MESH_HPP

#include "tree.hpp"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/polygon_soup_io.h>
#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/Real_timer.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/alpha_wrap_3.h>
#include <array>
#include <iostream>
#include <string>
#include <vector>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point_3 = K::Point_3;
using Mesh = CGAL::Surface_mesh<Point_3>;

class TreeMesh {
  public:
    void wrap_tree(const Tree &t) {
        std::string filePath = "../data/tree/";
        std::string season = t.getSeason();
        std::string species = t.getSpecies();

        if (season == "spring" || season == "summer") {
            filePath += "leafy/";
        } else {
            filePath += "leafless/";
        }

        // if (species == "Acer pseudoplatanus") {
        //     filePath += "arbre1.off";
        // }
        // ...
        filePath += "arbre1.off";
        filename = "arbre1.off";

        double relative_alpha;
        double relative_offset = 600;

        switch (M_lod) {
        case 0:
            relative_alpha = 0.1;
            break;
        case 1:
            relative_alpha = 20;
            break;
        case 2:
            relative_alpha = 100;
            break;
        default:
            break;
        }

        CGAL::data_file_path(filePath);
        std::vector<Point_3> points;
        std::vector<std::array<int, 3>> faces;
        std::vector<std::array<int, 3>> edges;
        CGAL::Bbox_3 bbox;

        if (!CGAL::IO::read_polygon_soup(filePath, points, faces) ||
            faces.empty()) {
            std::cerr << "Invalid input." << std::endl;
            exit(1);
        }

        for (const Point_3 &p : points)
            bbox += p.bbox();
        const double diag_length =
            std::sqrt(CGAL::square(bbox.xmax() - bbox.xmin()) +
                      CGAL::square(bbox.ymax() - bbox.ymin()) +
                      CGAL::square(bbox.zmax() - bbox.zmin()));

        const double alpha = diag_length / relative_alpha;
        const double offset = diag_length / relative_offset;

        CGAL::Real_timer ti;
        ti.start();

        CGAL::alpha_wrap_3(points, faces, alpha, offset, M_wrap);

        ti.stop();

        // std::cout << "Result: " << num_vertices(M_wrap) << " vertices, "
        //           << num_faces(M_wrap) << " faces" << std::endl;
        // std::cout << "Took " << ti.time() << " s." << std::endl;
    }

    void setLod(int lod) { M_lod = lod; }

    std::string getFilename() const { return filename; }
    int getLod() const { return M_lod; }
    Mesh getWrap() const { return M_wrap; }

    void dumpMesh() const {

        std::string output_dir = "../temp/";

        std::string input_name = std::string(filename);
        input_name = input_name.substr(input_name.find_last_of("/") + 1,
                                       input_name.length() - 1);
        input_name = input_name.substr(0, input_name.find_last_of("."));

        std::string output_name = output_dir + input_name + "_" +
                                  std::to_string(static_cast<int>(M_lod)) +
                                  "_" + +".off";
        // std::cout << "Writing to " << output_name << std::endl;
        CGAL::IO::write_polygon_mesh(output_name, M_wrap,
                                     CGAL::parameters::stream_precision(17));
    }

  private:
    int M_lod;
    Mesh M_wrap;
    std::string filename;
};

#endif