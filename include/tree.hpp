#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include <nlohmann/json.hpp>

class Tree
{
private:
    long id;
    double lat;
    double lon;
    std::string genus;
    std::string species;
    double height;
    double circumference;
    double diameter_crown;

public:
    // Default constructor
    Tree();

    // Constructor that takes arguments
    Tree(long id, double lat, double lon, std::string genus, std::string species, double height, double circumference, double diameter_crown);

    // Getters
    long getId() const { return id; }
    double getLat() const { return lat; }
    double getLon() const { return lon; }
    std::string getGenus() const { return genus; }
    std::string getSpecies() const { return species; }
    double getHeight() const { return height; }
    double getCircumference() const { return circumference; }
    double getDiameterCrown() const { return diameter_crown; }

    // Setters
    void setId(long id) { this->id = id; }
    void setLat(double lat) { this->lat = lat; }
    void setLon(double lon) { this->lon = lon; }
    void setGenus(std::string genus) { this->genus = genus; }
    void setSpecies(std::string species) { this->species = species; }
    void setHeight(double height) { this->height = height; }
    void setCircumference(double circumference) { this->circumference = circumference; }
    void setDiameterCrown(double diameter_crown) { this->diameter_crown = diameter_crown; }
};

Tree createTreeFromJson(const nlohmann::json &treeJson);

#endif