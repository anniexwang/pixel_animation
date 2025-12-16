#include "read_heart_json.h"
#include "../json/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool read_heart_json(
  const std::string & filename,
  std::vector<HeartPoint> & points
)
{
  // Check if file exists
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return false;
  }

  // Parse JSON
  json j;
  try {
    file >> j;
  } catch (const json::parse_error& e) {
    std::cerr << "Error: JSON parsing failed: " << e.what() << std::endl;
    return false;
  }

  // Validate that "points" array exists
  if (j.find("points") == j.end()) {
    std::cerr << "Error: JSON missing required field 'points'" << std::endl;
    return false;
  }

  if (!j["points"].is_array()) {
    std::cerr << "Error: 'points' field must be an array" << std::endl;
    return false;
  }

  // Clear output vector and reserve space
  points.clear();
  points.reserve(j["points"].size());

  // Extract each point
  for (size_t i = 0; i < j["points"].size(); i++) {
    const auto& point_json = j["points"][i];

    // Validate required fields exist
    if (point_json.find("x") == point_json.end() || 
        point_json.find("y") == point_json.end() ||
        point_json.find("r") == point_json.end() || 
        point_json.find("g") == point_json.end() ||
        point_json.find("b") == point_json.end()) {
      std::cerr << "Error: Point " << i << " missing required fields (x, y, r, g, b)" << std::endl;
      return false;
    }

    // Validate field types
    if (!point_json["x"].is_number() || !point_json["y"].is_number()) {
      std::cerr << "Error: Point " << i << " has invalid x or y coordinate type" << std::endl;
      return false;
    }

    if (!point_json["r"].is_number() || !point_json["g"].is_number() || !point_json["b"].is_number()) {
      std::cerr << "Error: Point " << i << " has invalid RGB value type" << std::endl;
      return false;
    }

    // Extract values
    HeartPoint point;
    point.x = point_json["x"].get<double>();
    point.y = point_json["y"].get<double>();

    // Extract RGB values and validate range [0, 255]
    int r_val = point_json["r"].get<int>();
    int g_val = point_json["g"].get<int>();
    int b_val = point_json["b"].get<int>();

    // Clamp RGB values to valid range
    if (r_val < 0 || r_val > 255 || g_val < 0 || g_val > 255 || b_val < 0 || b_val > 255) {
      std::cerr << "Warning: Point " << i << " has RGB values out of range [0, 255], clamping" << std::endl;
    }

    point.r = static_cast<unsigned char>(std::max(0, std::min(255, r_val)));
    point.g = static_cast<unsigned char>(std::max(0, std::min(255, g_val)));
    point.b = static_cast<unsigned char>(std::max(0, std::min(255, b_val)));

    points.push_back(point);
  }

  return true;
}
