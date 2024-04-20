#include "../include/tree.hpp"
#include "../include/WGS84toCartesian.hpp"
#include <string>

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
                           {M_lon, M_lat} /* position to be converted */);
    M_x = cartesianPosition[0];
    M_y = cartesianPosition[1];
}

void Tree::wrap(int lod) {
    std::string filename = "../tree_ref/";
    double scaling_factor;
    std::vector<Point_3> points;
    std::vector<std::array<int, 3>> faces;
    std::vector<std::array<int, 3>> edges;
    CGAL::Bbox_3 bbox;

    // Append LOD to filename
    filename += "arbre1_lod" + std::to_string(lod) + ".stl";

    CGAL::data_file_path(filename);

    if (!CGAL::IO::read_polygon_soup(filename, points, faces) ||
        faces.empty()) {
        std::cerr << "Invalid input." << std::endl;
        exit(1);
    }

    for (const Point_3 &p : points)
        bbox += p.bbox();

    // Calculate scaling factor based on tree height
    if (M_height == 0) {
        M_height = 5; // k nearest would be better
    }

    scaling_factor = M_height / (bbox.zmax() - bbox.zmin());
    std::cout << "x = " << x() << ", y=" << y() << ", tree height=" << M_height;

    // Translate mesh to tree coordinates and scale based on tree height
    for (auto &point : points) {
        point = Point_3(point.x() + x(), point.y() + y(), point.z());
        point = Point_3(point.x() * scaling_factor, point.y() * scaling_factor,
                        point.z() * scaling_factor);
    }

    // Store the modified mesh in M_M_wrap
    M_wrap.clear();
    CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, faces,
                                                                M_wrap);

    // Optionally, you can update the bounding box if needed
    bbox = CGAL::Polygon_mesh_processing::bbox(M_wrap);
    std::cout << ", new box height: " << bbox.zmax() - bbox.zmin() << std::endl;
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