#include "generate_star_points.h"
#include <cmath>
#include <random>

void generate_star_points(
  std::vector<StarPoint> & points,
  const int image_width,
  const int image_height,
  const int num_points
)
{
  points.clear();
  points.reserve(num_points);
  
  // Random number generator
  std::random_device rd;
  std::mt19937 gen(42);  // Fixed seed for consistency
  std::uniform_real_distribution<> unit_dist(0.0, 1.0);
  
  // Image center
  const double center_x = image_width / 2.0;
  const double center_y = image_height / 2.0;
  
  // Scale to make star large (similar to heart which uses scale=10 with max radius ~16)
  const double scale = 150.0;
  
  // Five-pointed star with straight edges and rounded tips
  const double outer_radius = 1.0;
  const double inner_radius = 0.382;  // Golden ratio approximation for aesthetic star
  
  // Define the 10 vertices of the star (5 outer points + 5 inner points)
  // Rotated 180 degrees so star points downward
  std::vector<std::pair<double, double>> vertices;
  for (int v = 0; v < 10; v++) {
    double angle = (v * M_PI / 5.0) + M_PI / 2.0;  // Start from bottom (180 degree rotation)
    double radius = (v % 2 == 0) ? outer_radius : inner_radius;
    vertices.push_back({radius * std::cos(angle), radius * std::sin(angle)});
  }
  
  // Generate points by filling the star shape
  for (int i = 0; i < num_points; i++) {
    // Pick a random triangle within the star
    // The star can be divided into 10 triangles from the center
    int triangle_idx = static_cast<int>(unit_dist(gen) * 10);
    
    // Get the two vertices of this triangle edge
    auto v1 = vertices[triangle_idx];
    auto v2 = vertices[(triangle_idx + 1) % 10];
    
    // Generate a random point in the triangle (center, v1, v2)
    // Using barycentric coordinates
    double r1 = unit_dist(gen);
    double r2 = unit_dist(gen);
    if (r1 + r2 > 1.0) {
      r1 = 1.0 - r1;
      r2 = 1.0 - r2;
    }
    
    double x = r1 * v1.first + r2 * v2.first;
    double y = r1 * v1.second + r2 * v2.second;
    
    // Check if we're near an outer tip (even triangle indices have outer vertices)
    bool is_outer_triangle = (triangle_idx % 2 == 0);
    
    // Round ONLY the outer tips by checking proximity to outer vertex
    if (is_outer_triangle) {
      auto outer_vertex = vertices[triangle_idx];
      double dx = x - outer_vertex.first;
      double dy = y - outer_vertex.second;
      double dist_to_tip = std::sqrt(dx * dx + dy * dy);
      
      // If very close to the tip (within 0.12 units), round it
      if (dist_to_tip < 0.12) {
        double round_factor = 1.0 - (dist_to_tip / 0.12);  // 1.0 at tip, 0.0 at boundary
        double pull_back = round_factor * 0.08;  // Pull back up to 8%
        x = x - dx * pull_back;
        y = y - dy * pull_back;
      }
    }
    
    // Add density bias and glow effect
    double rand_val = unit_dist(gen);
    
    if (rand_val < 0.12) {
      // 12% scattered outside for glowing effect
      double glow_factor = 1.0 + unit_dist(gen) * 0.35;
      x *= glow_factor;
      y *= glow_factor;
    } else if (rand_val < 0.28) {
      // 16% uniformly distributed (fill interior)
      // Keep as is
    } else {
      // 72% biased toward the triangle edges (not radially!)
      // Push toward a random point on the triangle edge
      double edge_bias = std::pow(unit_dist(gen), 0.10);
      double edge_param = unit_dist(gen);  // Random point along v1-v2 edge
      double edge_x = v1.first * (1.0 - edge_param) + v2.first * edge_param;
      double edge_y = v1.second * (1.0 - edge_param) + v2.second * edge_param;
      
      x = x * (1.0 - edge_bias) + edge_x * edge_bias;
      y = y * (1.0 - edge_bias) + edge_y * edge_bias;
    }
    
    // Add jitter to eliminate clustering patterns
    double jitter_amount = 0.10;
    x += (unit_dist(gen) - 0.5) * jitter_amount;
    y += (unit_dist(gen) - 0.5) * jitter_amount;
    
    // Transform to image coordinates
    double img_x = center_x + x * scale;
    double img_y = center_y - y * scale;  // Flip y
    
    // Skip if out of bounds
    if (img_x < 0 || img_x >= image_width || img_y < 0 || img_y >= image_height) {
      i--;  // Try again
      continue;
    }
    
    StarPoint point;
    point.x = img_x;
    point.y = img_y;
    
    // Color scattered randomly throughout - not based on position
    // Mix of yellow, orange, red, and white tones
    double color_choice = unit_dist(gen);
    
    if (color_choice < 0.20) {
      // 20% bright white/yellow-white
      point.r = 250 + static_cast<unsigned char>(unit_dist(gen) * 5);
      point.g = 245 + static_cast<unsigned char>(unit_dist(gen) * 10);
      point.b = 200 + static_cast<unsigned char>(unit_dist(gen) * 55);
    } else if (color_choice < 0.40) {
      // 20% bright yellow
      point.r = 255;
      point.g = 240 + static_cast<unsigned char>(unit_dist(gen) * 15);
      point.b = 100 + static_cast<unsigned char>(unit_dist(gen) * 100);
    } else if (color_choice < 0.60) {
      // 20% yellow-orange
      point.r = 255;
      point.g = 180 + static_cast<unsigned char>(unit_dist(gen) * 60);
      point.b = 40 + static_cast<unsigned char>(unit_dist(gen) * 80);
    } else if (color_choice < 0.75) {
      // 15% orange
      point.r = 240 + static_cast<unsigned char>(unit_dist(gen) * 15);
      point.g = 120 + static_cast<unsigned char>(unit_dist(gen) * 80);
      point.b = 20 + static_cast<unsigned char>(unit_dist(gen) * 50);
    } else if (color_choice < 0.88) {
      // 13% orange-red
      point.r = 220 + static_cast<unsigned char>(unit_dist(gen) * 35);
      point.g = 80 + static_cast<unsigned char>(unit_dist(gen) * 70);
      point.b = 15 + static_cast<unsigned char>(unit_dist(gen) * 35);
    } else if (color_choice < 0.96) {
      // 8% red-orange (deeper)
      point.r = 200 + static_cast<unsigned char>(unit_dist(gen) * 40);
      point.g = 60 + static_cast<unsigned char>(unit_dist(gen) * 60);
      point.b = 10 + static_cast<unsigned char>(unit_dist(gen) * 30);
    } else {
      // 4% faint glow (for outer particles)
      point.r = 80 + static_cast<unsigned char>(unit_dist(gen) * 60);
      point.g = 70 + static_cast<unsigned char>(unit_dist(gen) * 50);
      point.b = 20 + static_cast<unsigned char>(unit_dist(gen) * 30);
    }
    
    points.push_back(point);
  }
}
