#include "calculate_centroid.h"

void calculate_centroid(
  const std::vector<HeartPoint> & points,
  double & center_x,
  double & center_y
)
{
  ////////////////////////////////////////////////////////////////////////////
  // Calculate the geometric center (centroid) of all points
  ////////////////////////////////////////////////////////////////////////////
  
  center_x = 0.0;
  center_y = 0.0;
  
  // Handle empty point set
  if (points.empty()) {
    return;
  }
  
  // Sum all x and y coordinates
  for (const auto & point : points) {
    center_x += point.x;
    center_y += point.y;
  }
  
  // Calculate average
  const double num_points = static_cast<double>(points.size());
  center_x /= num_points;
  center_y /= num_points;
}

// Overload for StarPoint
#include "generate_star_points.h"

void calculate_centroid(
  const std::vector<StarPoint> & points,
  double & center_x,
  double & center_y
)
{
  center_x = 0.0;
  center_y = 0.0;
  
  // Handle empty point set
  if (points.empty()) {
    return;
  }
  
  // Sum all x and y coordinates
  for (const auto & point : points) {
    center_x += point.x;
    center_y += point.y;
  }
  
  // Calculate average
  const double num_points = static_cast<double>(points.size());
  center_x /= num_points;
  center_y /= num_points;
}
