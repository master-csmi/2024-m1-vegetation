#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>

class Config {
  private:
    std::string M_bbox;
    int M_LOD;
    std::string M_output_name;
    std::string M_default_genus;
    double M_default_height;
    std::string M_origin;
    std::string M_input_building_mesh;

  public:
    Config(std::string const &filename);

    ~Config();

    // Getters
    std::string output_name() const { return M_output_name; }
    std::string default_genus() const { return M_default_genus; }
    double default_height() const { return M_default_height; }
    std::string bbox() const { return M_bbox; }
    int LOD() const { return M_LOD; }
    std::vector<double> bbox_coords() const;
    std::string origin() const { return M_origin; }
    std::string input_building_mesh() const { return M_input_building_mesh; }
};

std::ostream &operator<<(std::ostream &os, const Config &config);
std::pair<double, double> extractCoordinates(const std::string &origin);

#endif