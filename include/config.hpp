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
    std::vector<double> bbox_coords() const {
        std::vector<double> coords;
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;
        std::string bbox = M_bbox; // Create a local copy of M_bbox
        while ((pos = bbox.find(delimiter)) != std::string::npos) {
            token = bbox.substr(0, pos);
            coords.push_back(std::stod(token));
            bbox.erase(0, pos + delimiter.length());
        }
        coords.push_back(std::stod(bbox));
        return coords;
    }
};

std::ostream &operator<<(std::ostream &os, const Config &config);

#endif