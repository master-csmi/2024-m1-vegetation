#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "query.hpp"
#include <string>

class Config {
  private:
    double A_lat, A_lon;
    double B_lat, B_lon;
    int M_LOD;

  public:
    Config(std::string const &filename);

    ~Config();

    // Getters
    double getAlat() const;
    double getAlon() const;
    double getBlat() const;
    double getBlon() const;
    double getLOD() const;
    Query get_query() const;

    friend std::ostream &operator<<(std::ostream &os, const Config &config);
};

#endif