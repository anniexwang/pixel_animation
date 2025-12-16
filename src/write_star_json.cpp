#include "write_star_json.h"
#include "../json/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool write_star_json(
  const std::string & filename,
  const std::vector<StarPoint> & points
)
{
  // Create JSON array
  json j;
  j["points"] = json::array();
  
  // Add each point to the array
  for (const auto & point : points) {
    json point_obj;
    point_obj["x"] = point.x;
    point_obj["y"] = point.y;
    point_obj["r"] = static_cast<int>(point.r);
    point_obj["g"] = static_cast<int>(point.g);
    point_obj["b"] = static_cast<int>(point.b);
    j["points"].push_back(point_obj);
  }
  
  // Write to file
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
    return false;
  }
  
  file << j.dump(2);  // Pretty print with 2-space indentation
  file.close();
  
  std::cout << "Wrote " << points.size() << " points to " << filename << std::endl;
  return true;
}
