#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>

/**
 * @brief Class to store the configuration of the program
 *
 */
class Config {
  private:
    std::string M_bbox;                ///< Bounding box of the area of interest
    int M_LOD;                         ///< Level of detail of the output mesh
    std::string M_output_name;         ///< Name of the output file
    std::string M_default_genus;       ///< Default genus when not specified
    std::string M_default_height;      ///< Default height when not specified
    std::string M_origin;              ///< Origin of the coordinates
    std::string M_input_building_mesh; ///< Path to the input building mesh
    double M_altitude;                 ///< Altitude of the area of interest
    bool M_merge;                      ///< Merge buildings and trees

  public:
    Config(std::string const &filename);

    ~Config();

    // Getters
    std::string output_name() const { return M_output_name; }
    std::string default_genus() const { return M_default_genus; }
    std::string default_height() const { return M_default_height; }
    std::string bbox() const { return M_bbox; }
    int LOD() const { return M_LOD; }
    std::vector<double> bbox_coords() const;
    std::string origin() const { return M_origin; }
    std::string input_building_mesh() const { return M_input_building_mesh; }
    double altitude() const { return M_altitude; }
    bool merge() const { return M_merge; }
};

std::ostream &operator<<(std::ostream &os, const Config &config);

/**
 * @brief Function to convert a string to a pair of doubles
 *
 * @param origin String to convert
 * @return std::pair<double, double> Pair of doubles
 */
std::pair<double, double> string_to_pair(const std::string &origin);

#endif