#ifndef CALCULATE_CENTROID_H
#define CALCULATE_CENTROID_H

#include "generate_heart_points.h"
#include "generate_star_points.h"
#include <vector>

// Calculate the geometric center (centroid) of a set of points
//
// Inputs:
//   points  vector of HeartPoint structures
//   center_x  output parameter for the x-coordinate of the centroid
//   center_y  output parameter for the y-coordinate of the centroid
//
// The centroid is calculated as:
//   center_x = sum(all x coordinates) / num_points
//   center_y = sum(all y coordinates) / num_points
void calculate_centroid(
  const std::vector<HeartPoint> & points,
  double & center_x,
  double & center_y
);

// Overload for StarPoint
void calculate_centroid(
  const std::vector<StarPoint> & points,
  double & center_x,
  double & center_y
);

#endif
