#ifndef TRANSFORM_STAR_POINTS_H
#define TRANSFORM_STAR_POINTS_H

#include <vector>
#include "generate_star_points.h"

// Transform star points by scaling them relative to a center point
//
// Inputs:
//   input_points  - vector of StarPoint to transform
//   center_x      - x coordinate of transformation center
//   center_y      - y coordinate of transformation center
//   scale_factor  - scaling factor (1.0 = no change, <1.0 = contract, >1.0 = expand)
// Outputs:
//   output_points - vector to store transformed StarPoint (will be resized)
//
// Formula: new_pos = center + scale_factor * (old_pos - center)
// Color values are preserved during transformation
void transform_star_points(
  const std::vector<StarPoint> & input_points,
  std::vector<StarPoint> & output_points,
  const double center_x,
  const double center_y,
  const double scale_factor
);

#endif
