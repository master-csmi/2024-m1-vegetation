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
    std::string M_default_genus;
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
    std::string default_genus() const { return M_default_genus; }
    double default_height() const { return M_default_height; }

    void setAlat(double Alat) { A_lat = Alat; }
    void setAlon(double Alon) { A_lon = Alon; }
    void setBlat(double Blat) { B_lat = Blat; }
    void setBlon(double Blon) { B_lon = Blon; }
};

std::ostream &operator<<(std::ostream &os, const Config &config);

#endif