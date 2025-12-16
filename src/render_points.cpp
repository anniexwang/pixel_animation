#include "render_points.h"
#include <cmath>

void render_points(
  std::vector<unsigned char> & image,
  const int width,
  const int height,
  const std::vector<HeartPoint> & points,
  const int point_radius
)
{
  // Initialize image with dark purple background (40, 20, 60)
  image.resize(width * height * 3);
  for (int i = 0; i < width * height; i++) {
    image[i * 3 + 0] = 40;  // R
    image[i * 3 + 1] = 20;  // G
    image[i * 3 + 2] = 60;  // B
  }

  // Render each point
  for (const auto& point : points) {
    if (point_radius == 1) {
      // Single pixel rendering
      int px = static_cast<int>(std::round(point.x));
      int py = static_cast<int>(std::round(point.y));

      // Skip out-of-bounds points
      if (px < 0 || px >= width || py < 0 || py >= height) {
        continue;
      }

      // Set pixel color
      int index = 3 * (py * width + px);
      image[index + 0] = point.r;
      image[index + 1] = point.g;
      image[index + 2] = point.b;
    } else {
      // Filled circle rendering using midpoint circle algorithm
      int cx = static_cast<int>(std::round(point.x));
      int cy = static_cast<int>(std::round(point.y));

      // Draw filled circle by scanning each row
      for (int dy = -point_radius; dy <= point_radius; dy++) {
        int py = cy + dy;
        if (py < 0 || py >= height) continue;

        // Calculate horizontal extent at this y
        int dx_max = static_cast<int>(std::sqrt(point_radius * point_radius - dy * dy));
        
        for (int dx = -dx_max; dx <= dx_max; dx++) {
          int px = cx + dx;
          if (px < 0 || px >= width) continue;

          // Set pixel color
          int index = 3 * (py * width + px);
          image[index + 0] = point.r;
          image[index + 1] = point.g;
          image[index + 2] = point.b;
        }
      }
    }
  }
}

// Overload for StarPoint
#include "generate_star_points.h"

void render_points(
  std::vector<unsigned char> & image,
  const int width,
  const int height,
  const std::vector<StarPoint> & points,
  const int point_radius
)
{
  // Initialize image with dark purple background (40, 20, 60)
  image.resize(width * height * 3);
  for (int i = 0; i < width * height; i++) {
    image[i * 3 + 0] = 40;  // R
    image[i * 3 + 1] = 20;  // G
    image[i * 3 + 2] = 60;  // B
  }

  // Render each point
  for (const auto& point : points) {
    if (point_radius == 1) {
      // Single pixel rendering
      int px = static_cast<int>(std::round(point.x));
      int py = static_cast<int>(std::round(point.y));

      // Skip out-of-bounds points
      if (px < 0 || px >= width || py < 0 || py >= height) {
        continue;
      }

      // Set pixel color
      int index = 3 * (py * width + px);
      image[index + 0] = point.r;
      image[index + 1] = point.g;
      image[index + 2] = point.b;
    } else {
      // Filled circle rendering using midpoint circle algorithm
      int cx = static_cast<int>(std::round(point.x));
      int cy = static_cast<int>(std::round(point.y));

      // Draw filled circle by scanning each row
      for (int dy = -point_radius; dy <= point_radius; dy++) {
        int py = cy + dy;
        if (py < 0 || py >= height) continue;

        // Calculate horizontal extent at this y
        int dx_max = static_cast<int>(std::sqrt(point_radius * point_radius - dy * dy));
        
        for (int dx = -dx_max; dx <= dx_max; dx++) {
          int px = cx + dx;
          if (px < 0 || px >= width) continue;

          // Set pixel color
          int index = 3 * (py * width + px);
          image[index + 0] = point.r;
          image[index + 1] = point.g;
          image[index + 2] = point.b;
        }
      }
    }
  }
}
