#ifndef GENERATE_STAR_POINTS_H
#define GENERATE_STAR_POINTS_H

#include <vector>

// Structure representing a colored point in 2D space
struct StarPoint {
  double x;           // X coordinate
  double y;           // Y coordinate
  unsigned char r;    // Red channel [0-255]
  unsigned char g;    // Green channel [0-255]
  unsigned char b;    // Blue channel [0-255]
};

// Generate a star-shaped distribution of colored points
//
// Inputs:
//   points  output vector to store generated star points
//   image_width  width of the target image canvas
//   image_height  height of the target image canvas
//   num_points  approximate number of points to generate
//
// The function generates points along a parametric five-pointed star curve with:
// - Random offsets for particle effect
// - Color gradient from bright yellow/white (edges) to orange/red (center)
// - Varying density distribution
void generate_star_points(
  std::vector<StarPoint> & points,
  const int image_width,
  const int image_height,
  const int num_points
);

#endif
