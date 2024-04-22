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
    bool M_new_query;
    double M_distKNN;

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
    int new_query() const;
    double distKNN() const;
};

std::ostream &operator<<(std::ostream &os, const Config &config);

#endif