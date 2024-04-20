#ifndef TREE_HPP
#define TREE_HPP

#include "WGS84toCartesian.hpp"
#include "json.hpp"
#include "query.hpp"
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

class TreeMesh;
class Tree {
  protected:
    long id;
    double lat;
    double lon;
    std::string genus;
    std::string species;
    std::string season;
    double height;
    double circumference;
    double diameter_crown;
    double x, y;
    Mesh M_wrap;

  public:
    // Default constructor
    Tree();

    // Constructor that takes arguments
    Tree(long id, double lat, double lon, std::string genus,
         std::string species, double height, double circumference,
         double diameter_crown);

    // Getters
    long getId() const { return id; }
    double getLat() const { return lat; }
    double getLon() const { return lon; }
    std::string getGenus() const { return genus; }
    std::string getSpecies() const { return species; }
    double getHeight() const { return height; }
    double getCircumference() const { return circumference; }
    double getDiameterCrown() const { return diameter_crown; }
    std::string getSeason() const { return season; }
    double getX() const { return x; }
    double getY() const { return y; }
    Mesh getWrap() const { return M_wrap; }

    // Setters
    void setId(long id) { this->id = id; }
    void setLat(double lat) { this->lat = lat; }
    void setLon(double lon) { this->lon = lon; }
    void setGenus(std::string genus) { this->genus = genus; }
    void setSpecies(std::string species) { this->species = species; }
    void setHeight(double height) { this->height = height; }
    void setCircumference(double circumference) {
        this->circumference = circumference;
    }
    void setDiameterCrown(double diameter_crown) {
        this->diameter_crown = diameter_crown;
    }
    void computeXY(const Query &q) {
        std::array<double, 2> ref = q.getRefPt();
        std::array<double, 2> cartesianPosition =
            wgs84::toCartesian({ref[0], ref[1]} /* reference position */,
                               {lon, lat} /* position to be converted */);
        x = cartesianPosition[0];
        y = cartesianPosition[1];
    }
    void wrap(int lod);
};

Tree createTreeFromJson(const nlohmann::json &treeJson);

#endif