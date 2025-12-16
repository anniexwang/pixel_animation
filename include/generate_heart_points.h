#ifndef GENERATE_HEART_POINTS_H
#define GENERATE_HEART_POINTS_H

#include <vector>

// Structure representing a colored point in 2D space
struct HeartPoint {
  double x;           // X coordinate
  double y;           // Y coordinate
  unsigned char r;    // Red channel [0-255]
  unsigned char g;    // Green channel [0-255]
  unsigned char b;    // Blue channel [0-255]
};

// Generate a heart-shaped distribution of colored points
//
// Inputs:
//   points  output vector to store generated heart points
//   image_width  width of the target image canvas
//   image_height  height of the target image canvas
//   num_points  approximate number of points to generate
//
// The function generates points along a parametric heart curve with:
// - Random offsets for particle effect
// - Color gradient from bright pink/white (center) to dark red/pink (edges)
// - Denser distribution at edges, sparser in center
void generate_heart_points(
  std::vector<HeartPoint> & points,
  const int image_width,
  const int image_height,
  const int num_points
);

#endif
