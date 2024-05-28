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

/**
 * @brief Class to store the information of a tree
 *
 */
class Tree {
  private:
    long M_id;                     ///< Id of the tree
    double M_lat, M_lon, M_x, M_y; ///< Latitude, Longitude, x and y coordinates
    double M_height, M_altitude;   ///< Height and altitude of the tree
    double M_circumference,
        M_diameter_crown; ///< Circumference and diameter of the crown
    std::string M_genus, M_species,
        M_season; ///< Genus, species, season of the tree
    std::vector<std::string> M_known_genus, M_cedrus_like, M_acer_like,
        M_liquidambar_like, M_quercus_like;
    Mesh M_wrap;                             ///< Wrapped mesh of the tree
    std::vector<Point_3> M_points;           ///< Points of the tree mesh
    std::vector<std::array<int, 3>> M_faces; ///< Faces of the tree mesh

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

    /**
     * @brief Convert latitude and longitude to x and y coordinates
     *
     * @param ref_lat A reference latitude to use as origin
     * @param ref_lon A reference longitude to use as origin
     */
    void computeXY(double ref_lat, double ref_lon);

    /**
     * @brief Wrap the tree mesh
     *
     * @param lod Level of detail of the output mesh
     * @param rep Directory where the base mesh model is stored
     */
    void wrap(int lod, std::string rep = "./");

    /**
     * @brief Load genus data from a file
     *
     * @param filename Path to the file
     */
    void load_genus(const std::string &filename);
};

/**
 * @brief Create a Tree from a json object
 *
 * @param treeJson a nlohmann::json object
 * @return Tree a Tree object
 */
Tree createTreeFromJson(const nlohmann::json &treeJson);

/**
 * @brief Compare two trees by their height
 *
 * @param lhs a Tree object
 * @param rhs a Tree object
 * @return true if rhs is taller than lhs
 * @return false otherwise
 */
bool operator<(const Tree &lhs, const Tree &rhs);

std::ostream &operator<<(std::ostream &os, const Tree &tree);

#endif