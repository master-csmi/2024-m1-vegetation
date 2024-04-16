#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "query.hpp"

class Config
{
private:
    double A_lat, A_lon;
    double B_lat, B_lon;
public:
    // Constructor
    Config(std::string const& filename);

    // Destructor
    ~Config();

    // Getters
    double getAlat() const;
    double getAlon() const;
    double getBlat() const;
    double getBlon() const;

    // Get the query
    Query get_query() const;
};

#endif