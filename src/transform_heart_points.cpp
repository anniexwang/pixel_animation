#include "transform_heart_points.h"
#include <cmath>

// Helper function to calculate distance to heart curve at a given angle
double heart_distance_at_angle(double angle, double scale) {
  // Parametric heart curve equations (same as in generate_heart_points)
  double x_curve = 16.0 * std::pow(std::sin(angle), 3);
  double y_curve = 13.0 * std::cos(angle) 
                 - 5.0 * std::cos(2.0 * angle) 
                 - 2.0 * std::cos(3.0 * angle) 
                 - std::cos(4.0 * angle);
  
  // Round the bottom (same as generation)
  if (y_curve < -12.0) {
    double excess = -12.0 - y_curve;
    y_curve = -12.0 - excess * 0.05;
  }
  
  // Return distance from origin to curve at this angle
  return std::sqrt(x_curve * x_curve + y_curve * y_curve) * scale;
}

void transform_heart_points(
  const std::vector<HeartPoint> & original_points,
  std::vector<HeartPoint> & transformed_points,
  const double center_x,
  const double center_y,
  const double contraction_factor
)
{
  ////////////////////////////////////////////////////////////////////////////
  // Transform points with heart-shape-based distance:
  // - Calculate each point's distance relative to heart curve (not circle)
  // - Outer points (near heart edge) move more
  // - Inner points move less or opposite direction
  ////////////////////////////////////////////////////////////////////////////
  
  // Resize output vector to match input size
  transformed_points.resize(original_points.size());
  
  // Scale factor used in generation (must match generate_heart_points.cpp)
  const double scale = 10.0;
  
  // Transform each point based on its distance from center relative to heart shape
  for (size_t i = 0; i < original_points.size(); i++) {
    const HeartPoint & original = original_points[i];
    HeartPoint & transformed = transformed_points[i];
    
    // Calculate displacement from center
    const double dx = original.x - center_x;
    const double dy = -(original.y - center_y); // Flip y back to match generation
    
    // Calculate angle from center
    const double angle = std::atan2(dy, dx);
    
    // Calculate actual distance from center
    const double actual_distance = std::sqrt(dx * dx + dy * dy);
    
    // Calculate expected distance to heart curve at this angle
    const double heart_curve_distance = heart_distance_at_angle(angle, scale);
    
    // Normalize distance relative to heart shape (0 = center, 1 = on curve, >1 = outside)
    const double normalized_distance = (heart_curve_distance > 0) 
      ? (actual_distance / heart_curve_distance) 
      : 0.0;
    
    // Calculate distance-based scaling factor for more natural pumping
    // Outer points follow contraction_factor closely
    // Inner points move less but still visibly
    // This creates a natural "squeeze" effect
    
    // Clamp normalized_distance to reasonable range
    const double clamped_distance = std::min(1.2, std::max(0.0, normalized_distance));
    
    // Use a smooth curve to blend from reduced movement (center) to full movement (edge)
    // Power of 1.8 creates a smooth, natural transition
    const double distance_weight = std::pow(clamped_distance / 1.2, 1.8);
    
    // Inner points move about 85% as much as outer points (increased from 80%)
    // Outer points follow contraction_factor fully
    // This ensures ALL points move VERY visibly while maintaining natural pumping
    const double inner_scale_ratio = 0.85; // Inner points move 85% as much
    const double min_scale = 1.0 - (1.0 - contraction_factor) * inner_scale_ratio;
    const double point_scale = min_scale * (1.0 - distance_weight) + contraction_factor * distance_weight;
    
    // Apply the calculated scale (flip y back for image coordinates)
    transformed.x = center_x + dx * point_scale;
    transformed.y = center_y - dy * point_scale;
    
    // Preserve colors
    transformed.r = original.r;
    transformed.g = original.g;
    transformed.b = original.b;
  }
}
