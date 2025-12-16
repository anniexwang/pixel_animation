#include "transform_star_points.h"

void transform_star_points(
  const std::vector<StarPoint> & input_points,
  std::vector<StarPoint> & output_points,
  const double center_x,
  const double center_y,
  const double scale_factor
)
{
  // Resize output vector to match input size
  output_points.resize(input_points.size());
  
  // Transform each point
  for (size_t i = 0; i < input_points.size(); i++) {
    const StarPoint & in_point = input_points[i];
    StarPoint & out_point = output_points[i];
    
    // Apply scaling transformation relative to center
    // Formula: new_pos = center + scale_factor * (old_pos - center)
    out_point.x = center_x + scale_factor * (in_point.x - center_x);
    out_point.y = center_y + scale_factor * (in_point.y - center_y);
    
    // Preserve color values
    out_point.r = in_point.r;
    out_point.g = in_point.g;
    out_point.b = in_point.b;
  }
}
