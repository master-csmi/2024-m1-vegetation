#include "../include/tree.hpp"
#include "../include/WGS84toCartesian.hpp"
#include <CGAL/Aff_transformation_3.h>
#include <string>

namespace PMP = CGAL::Polygon_mesh_processing;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Vector_3 Vector_3;

// Default constructor
Tree::Tree()
    : M_id(0), M_lat(0), M_lon(0), M_height(0), M_circumference(0),
      M_diameter_crown(0) {}

// Constructor that takes arguments
Tree::Tree(long id, double lat, double lon, std::string genus,
           std::string species, double height, double circumference,
           double diameter_crown)
    : M_id(id), M_lat(lat), M_lon(lon), M_genus(genus), M_species(species),
      M_height(height), M_circumference(circumference),
      M_diameter_crown(diameter_crown) {}

Tree createTreeFromJson(const nlohmann::json &treeJson) {
    Tree tree;

    if (treeJson.contains("id")) {
        tree.setId(treeJson["id"].get<int>());
    }

    if (treeJson.contains("lat")) {
        tree.setLat(treeJson["lat"].get<double>());
    }

    if (treeJson.contains("lon")) {
        tree.setLon(treeJson["lon"].get<double>());
    }

    if (treeJson.contains("tags")) {
        if (treeJson["tags"].contains("genus")) {
            tree.setGenus(treeJson["tags"]["genus"].get<std::string>());
        }

        if (treeJson["tags"].contains("species")) {
            tree.setSpecies(treeJson["tags"]["species"].get<std::string>());
        }

        if (treeJson["tags"].contains("height")) {
            tree.setHeight(
                std::stod(treeJson["tags"]["height"].get<std::string>()));
        }

        if (treeJson["tags"].contains("circumference")) {
            tree.setCircumference(std::stod(
                treeJson["tags"]["circumference"].get<std::string>()));
        }

        if (treeJson["tags"].contains("diameter_crown")) {
            tree.setDiameterCrown(std::stod(
                treeJson["tags"]["diameter_crown"].get<std::string>()));
        }
    }

    return tree;
}

void Tree::computeXY(double ref_lat, double ref_lon) {
    std::array<double, 2> cartesianPosition =
        wgs84::toCartesian({ref_lat, ref_lon} /* reference position */,
                           {M_lat, M_lon} /* position to be converted */);
    M_x = cartesianPosition[0];
    M_y = cartesianPosition[1];
}

void Tree::wrap(int lod) {
    std::string filename = "../tree_ref/";
    double scaling_factor_double;
    std::vector<Point_3> points;
    std::vector<std::array<int, 3>> faces;
    CGAL::Bbox_3 bbox;

    // Append LOD to filename
    filename += "Platanus_lod" + std::to_string(lod) + ".stl";

    CGAL::data_file_path(filename);

    if (!CGAL::IO::read_STL(filename, points, faces)) {
        std::cerr << "Invalid input." << std::endl;
        exit(1);
    }

    // Calculate bounding box from points
    for (const Point_3 &p : points)
        bbox += p.bbox();

    if (M_height == 0)
        M_height = 5; // change to k-nearest neighbors

    scaling_factor_double = M_height / (bbox.zmax() - bbox.zmin());

    K::RT scaling_factor(scaling_factor_double); // Convert to exact type

    // Create affine transformation (translation and scaling)
    CGAL::Aff_transformation_3<K> transformation =
        CGAL::Aff_transformation_3<K>(CGAL::TRANSLATION,
                                      Vector_3(M_x, M_y, 0)) *
        CGAL::Aff_transformation_3<K>(CGAL::SCALING, scaling_factor);

    // Apply transformation to each point in the mesh
    for (auto &p : points) {
        p = transformation.transform(p);
    }

    // Clear existing mesh data
    M_wrap.clear();

    // Add transformed vertices to the mesh and store their descriptors
    std::map<Point_3, Mesh::Vertex_index> vertex_map;
    for (const auto &p : points) {
        auto v = M_wrap.add_vertex(p);
        // Store the vertex descriptor for the transformed vertex
        vertex_map[p] = v;
    }

    // Add faces to the mesh
    for (const auto &face : faces) {
        // Retrieve vertex descriptors for the face vertices
        Mesh::Vertex_index v0 = vertex_map[points[face[0]]];
        Mesh::Vertex_index v1 = vertex_map[points[face[1]]];
        Mesh::Vertex_index v2 = vertex_map[points[face[2]]];

        // Add the face to the mesh
        M_wrap.add_face(v0, v1, v2);
    }

    // Compute the bounding box of the transformed mesh
    bbox = PMP::bbox(M_wrap);

    // std::cout << "\n height: " << M_height << std::endl;
    // std::cout << "x, y: " << M_x << ", " << M_y << std::endl;
    // std::cout << "New box height: " << bbox.zmax() - bbox.zmin() <<
    // std::endl;
}

std::ostream &operator<<(std::ostream &os, const Tree &tree) {
    os << "ID: " << tree.id() << std::endl;
    os << "Lat: " << tree.lat() << ", Lon: " << tree.lon() << std::endl;
    os << "Genus: " << tree.genus() << ", Species: " << tree.species()
       << std::endl;
    os << "Season: " << tree.season() << std::endl;
    os << "Height: " << tree.height()
       << ", Circumference: " << tree.circumference() << std::endl;
    os << "Diameter crown: " << tree.diameterCrown() << std::endl;
    os << "x: " << tree.x() << ", y: " << tree.y() << std::endl;
    return os;
}

bool operator<(const Tree &lhs, const Tree &rhs) {
    return lhs.height() < rhs.height();
}