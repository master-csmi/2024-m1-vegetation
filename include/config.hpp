#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "query.hpp"
#include <string>

class Config {
  private:
    double A_lat, A_lon;
    double B_lat, B_lon;
    int M_LOD;
    std::string M_output_name;
    double M_default_height;

  public:
    Config(std::string const &filename);

    ~Config();

    // Getters
    double Alat() const;
    double Alon() const;
    double Blat() const;
    double Blon() const;
    double LOD() const;
    Query query() const;
    std::string output_name() const;
    double default_height() const { return M_default_height; }
};

std::ostream &operator<<(std::ostream &os, const Config &config);

#endif