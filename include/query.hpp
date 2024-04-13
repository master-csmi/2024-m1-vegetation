#ifndef QUERY_HPP
#define QUERY_HPP

#include <json.hpp>

class Query {
  private:
    double min_lat;
    double max_lat;
    double min_lon;
    double max_lon;

  public:
    // Constructor
    Query(double Alat, double Alon, double Blat, double Blon);

    // Destructor
    ~Query();

    // Perform the Overpass query
    void perform_query();

    nlohmann::json get_query_result();
};

#endif