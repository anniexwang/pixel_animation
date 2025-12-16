#ifndef TRANSFORM_HEART_POINTS_H
#define TRANSFORM_HEART_POINTS_H

#include "generate_heart_points.h"
#include <vector>

// Transform heart points by applying a contraction factor toward a center point
//
// Inputs:
//   original_points  input vector of HeartPoint structures
//   transformed_points  output vector to store transformed points
//   center_x  x-coordinate of the contraction center
//   center_y  y-coordinate of the contraction center
//   contraction_factor  scaling factor for the transformation (0.0 to 1.0)
//                       1.0 = no change, < 1.0 = contract toward center
//
// The transformation applies:
//   new_x = center_x + (x - center_x) * contraction_factor
//   new_y = center_y + (y - center_y) * contraction_factor
//
// Colors are preserved from the original points.
void transform_heart_points(
  const std::vector<HeartPoint> & original_points,
  std::vector<HeartPoint> & transformed_points,
  const double center_x,
  const double center_y,
  const double contraction_factor
);

#endif
