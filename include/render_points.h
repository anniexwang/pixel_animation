#ifndef RENDER_POINTS_H
#define RENDER_POINTS_H

#include <vector>
#include "generate_heart_points.h"
#include "generate_star_points.h"

// Render colored points to an image buffer
//
// Inputs:
//   image  flattened RGB image buffer (width * height * 3 elements)
//   width  image width in pixels
//   height  image height in pixels
//   points  vector of HeartPoint structures to render
//   point_radius  radius of each point (1 = single pixel, >1 = filled circle)
//
// The function:
// - Initializes the image with dark purple background (40, 20, 60)
// - Renders each point at its position with its specified color
// - Skips points that fall outside image bounds
// - Uses simple overwrite (no blending) for performance
void render_points(
  std::vector<unsigned char> & image,
  const int width,
  const int height,
  const std::vector<HeartPoint> & points,
  const int point_radius
);

// Overload for StarPoint
void render_points(
  std::vector<unsigned char> & image,
  const int width,
  const int height,
  const std::vector<StarPoint> & points,
  const int point_radius
);

#endif
