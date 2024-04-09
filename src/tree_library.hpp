#ifndef TREE_LIBRARY_H
#define TREE_LIBRARY_H

#include <string>

class Tree
{
public:
    long id;
    double lat;
    double lon;
    std::string genus;
    std::string species;
    double height;
    double circumference;
    double diameter_crown;

    Tree(long id, double lat, double lon, std::string genus, std::string species, double height, double circumference, double diameter_crown)
        : id(id), lat(lat), lon(lon), genus(genus), species(species), height(height), circumference(circumference), diameter_crown(diameter_crown) {}
};

#endif
