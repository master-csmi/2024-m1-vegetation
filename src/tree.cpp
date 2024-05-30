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

void Tree::load_genus(const std::string &filename) {
    std::ifstream ifile(filename);
    if (ifile) {
        nlohmann::json data = nlohmann::json::parse(ifile);
        M_known_genus = data["known_genus"];
        M_cedrus_like = data["cedrus_like"];
        M_acer_like = data["acer_like"];
        M_liquidambar_like = data["liquidambar_like"];
        M_quercus_like = data["quercus_like"];
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

void Tree::wrap(int lod, std::string rep) {
    std::string filename = rep + "tree_ref/";
    double scaling_factor_double;
    CGAL::Bbox_3 bbox;

    if (std::find(M_known_genus.begin(), M_known_genus.end(), M_genus) !=
        M_known_genus.end()) {
        filename += M_genus;
    } else if (std::find(M_cedrus_like.begin(), M_cedrus_like.end(), M_genus) !=
               M_cedrus_like.end()) {
        filename += "Cedrus";
    } else if (std::find(M_acer_like.begin(), M_acer_like.end(), M_genus) !=
               M_acer_like.end()) {
        filename += "Acer";
    } else if (std::find(M_liquidambar_like.begin(), M_liquidambar_like.end(),
                         M_genus) != M_liquidambar_like.end()) {
        filename += "Liquidambar";
    } else if (std::find(M_quercus_like.begin(), M_quercus_like.end(),
                         M_genus) != M_quercus_like.end()) {
        filename += "Quercus";
    } else {
        // std::cout << "Genus : " << M_genus
        //           << ", not found in trees.json database, using Quercus
        //           instead"
        //           << std::endl;
        filename += "Quercus";
    }

    switch (lod) {
    case 0:
        filename += "_0_600.stl";
        break;
    case 1:
        filename += "_20_600.stl";
        break;
    case 2:
        filename += "_50_600.stl";
        break;
    case 3:
        filename += "_100_600.stl";
        break;
    default:
        std::cerr << "Invalid LOD." << std::endl;
        exit(1);
    }

    CGAL::data_file_path(filename);

    if (!CGAL::IO::read_polygon_soup(filename, M_points, M_faces)) {
        std::cerr << "Invalid input." << std::endl;
        exit(1);
    }

    // Calculate centroid of the tree
    double centroid_x = 0, centroid_y = 0, centroid_z = 0;
    for (const Point_3 &p : M_points) {
        centroid_x += p.x();
        centroid_y += p.y();
        centroid_z += p.z();
    }
    centroid_x /= M_points.size();
    centroid_y /= M_points.size();
    centroid_z /= M_points.size();
    Point_3 centroid(centroid_x, centroid_y, centroid_z);

    // Calculate bounding box from points
    for (const Point_3 &p : M_points)
        bbox += p.bbox();

    scaling_factor_double = M_height / (bbox.zmax() - bbox.zmin());

    K::RT scaling_factor(scaling_factor_double); // Convert to exact type

    // Find the base of the tree (minimum z-coordinate)
    // double base_z = std::numeric_limits<double>::max();
    // for (const auto &p : points) {
    //     if (p.z() < base_z)
    //         base_z = p.z();
    // }

    // Create affine transformations
    CGAL::Aff_transformation_3<K> translate_to_base(
        CGAL::TRANSLATION, Vector_3(-centroid.x(), -centroid.y(), M_altitude));
    CGAL::Aff_transformation_3<K> scale(CGAL::SCALING, scaling_factor);
    CGAL::Aff_transformation_3<K> translate_back(
        CGAL::TRANSLATION, Vector_3(centroid.x(), centroid.y(), M_altitude));
    CGAL::Aff_transformation_3<K> translate_to_target(
        CGAL::TRANSLATION, Vector_3(M_x, M_y, M_altitude));

    // Apply transformations: move to base, scale, move back, move to target
    for (auto &p : M_points) {
        p = translate_to_base.transform(p);   // Move to base
        p = scale.transform(p);               // Scale
        p = translate_back.transform(p);      // Move back to original position
        p = translate_to_target.transform(p); // Move to target position
    }
    // Clear existing mesh data
    M_wrap.clear();

    // Add transformed vertices to the mesh and store their descriptors
    std::map<Point_3, Mesh::Vertex_index> vertex_map;
    for (const auto &p : M_points) {
        auto v = M_wrap.add_vertex(p);
        // Store the vertex descriptor for the transformed vertex
        vertex_map[p] = v;
    }

    // Add faces to the mesh
    for (const auto &face : M_faces) {
        // Retrieve vertex descriptors for the face vertices
        Mesh::Vertex_index v0 = vertex_map[M_points[face[0]]];
        Mesh::Vertex_index v1 = vertex_map[M_points[face[1]]];
        Mesh::Vertex_index v2 = vertex_map[M_points[face[2]]];

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