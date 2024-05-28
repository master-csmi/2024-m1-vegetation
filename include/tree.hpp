#ifndef TREE_HPP
#define TREE_HPP

#include "json.hpp"
#include "json_helpers.hpp"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/polygon_soup_io.h>
#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/Polygon_mesh_processing/transform.h>
#include <CGAL/Real_timer.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/alpha_wrap_3.h>
#include <iostream>
#include <string>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point_3 = K::Point_3;
using Mesh = CGAL::Surface_mesh<Point_3>;

class Tree {
  private:
    long M_id;
    double M_lat, M_lon, M_x, M_y;
    double M_height, M_altitude;
    double M_circumference, M_diameter_crown;
    Mesh M_wrap;
    std::string M_genus, M_species, M_season;
    std::vector<std::string> M_known_genus, M_cedrus_like, M_acer_like,
        M_liquidambar_like, M_quercus_like;
    std::vector<Point_3> M_points;
    std::vector<std::array<int, 3>> M_faces;

  public:
    // Default constructor
    Tree();

    // Constructor that takes arguments
    Tree(long id, double lat, double lon, std::string genus,
         std::string species, double height, double circumference,
         double diameter_crown);

    // Getters
    long id() const { return M_id; }
    double lat() const { return M_lat; }
    double lon() const { return M_lon; }
    std::string genus() const { return M_genus; }
    std::string species() const { return M_species; }
    double height() const { return M_height; }
    double circumference() const { return M_circumference; }
    double diameterCrown() const { return M_diameter_crown; }
    std::string season() const { return M_season; }
    double x() const { return M_x; }
    double y() const { return M_y; }
    Mesh wrap() const { return M_wrap; }
    double altitude() const { return M_altitude; }
    std::vector<Point_3> points() const { return M_points; }
    std::vector<std::array<int, 3>> faces() const { return M_faces; }

    // Setters
    void setId(long id) { M_id = id; }
    void setLat(double lat) { M_lat = lat; }
    void setLon(double lon) { M_lon = lon; }
    void setGenus(std::string genus) { M_genus = genus; }
    void setSpecies(std::string species) { M_species = species; }
    void setHeight(double height) { M_height = height; }
    void setCircumference(double circumference) {
        M_circumference = circumference;
    }
    void setDiameterCrown(double diameter_crown) {
        M_diameter_crown = diameter_crown;
    }
    void setSeason(std::string season) { M_season = season; }
    void setAltitude(double altitude) { M_altitude = altitude; }

    void computeXY(double ref_lat, double ref_lon);
    void wrap(int lod, std::string rep = "./");
    void load_data(const std::string &filename);
};

Tree createTreeFromJson(const nlohmann::json &treeJson);
std::ostream &operator<<(std::ostream &os, const Tree &tree);
bool operator<(const Tree &lhs, const Tree &rhs);

#endif