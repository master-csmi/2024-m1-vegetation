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
};

std::ostream &operator<<(std::ostream &os, const Config &config);

#endif