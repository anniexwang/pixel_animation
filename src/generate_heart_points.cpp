#include "generate_heart_points.h"
#include <cmath>
#include <random>

void generate_heart_points(
  std::vector<HeartPoint> & points,
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
  
  // Scale to make heart large
  const double scale = 10.0;
  
  // Generate points by sampling along parametric heart curve
  // then filling inward with varying density
  for (int i = 0; i < num_points; i++) {
    // Random parameter t for heart curve
    double t = unit_dist(gen) * 2.0 * M_PI;
    
    // Parametric heart curve equations (modified for rounder bottom)
    double x_curve = 16.0 * std::pow(std::sin(t), 3);
    double y_curve = 13.0 * std::cos(t) 
                   - 5.0 * std::cos(2.0 * t) 
                   - 2.0 * std::cos(3.0 * t) 
                   - std::cos(4.0 * t);
    
    // Round the bottom significantly by cutting off the point
    // Make it even shorter and rounder
    if (y_curve < -12.0) {
      double excess = -12.0 - y_curve;
      y_curve = -12.0 - excess * 0.05;  // Very soft, short point (95% reduction)
    }
    
    // Check if we're at special regions: bottom tip or top area
    // Bottom tip: y < -10 (bottom area, adjusted for shorter tip)
    // Top area: y > 8 (top area where square pattern appears)
    bool is_bottom_tip = (y_curve < -10.0);
    bool is_top_area = (y_curve > 8.0);
    
    // Random distance from curve (0 to 1, biased toward curve)
    // Dense outline + scattered interior + extra scatter at bottom
    double rand_val = unit_dist(gen);
    double distance_factor;
    
    if (is_bottom_tip) {
      // Bottom tip: lots of scattered points
      if (rand_val < 0.20) {
        // 20% scattered outside
        distance_factor = 1.0 + unit_dist(gen) * 0.25;
      } else if (rand_val < 0.45) {
        // 25% scattered in middle range
        distance_factor = 0.3 + unit_dist(gen) * 0.5;
      } else {
        // 55% concentrated near outline but with more scatter
        distance_factor = 0.6 + std::pow(unit_dist(gen), 0.35) * 0.5;
      }
    } else {
      // Normal regions: VERY DENSE outline + fill interior + GLOW
      if (rand_val < 0.12) {
        // 12% scattered OUTSIDE for glowing effect (1.0 to 1.4)
        distance_factor = 1.0 + unit_dist(gen) * 0.4;
      } else if (rand_val < 0.25) {
        // 13% uniformly distributed from CENTER to everywhere (0 to 1.0)
        // This completely fills the interior including the top area
        distance_factor = unit_dist(gen) * 1.0;
      } else {
        // 75% concentrated near the outline (VERY DENSE EDGE)
        // Use very low power to push most points to the edge
        distance_factor = std::pow(unit_dist(gen), 0.08) * 1.0;
      }
    }
    
    // Scale curve point by distance factor
    double x = x_curve * distance_factor;
    double y = y_curve * distance_factor;
    
    // Add jitter to eliminate patterns
    // Extra jitter for bottom and top to break up any visible patterns
    double jitter_amount;
    if (is_bottom_tip) {
      jitter_amount = 8.0;  // Maximum jitter at bottom
    } else if (is_top_area) {
      jitter_amount = 9.0;  // MAXIMUM jitter at top to completely eliminate square pattern
    } else {
      jitter_amount = 6.0;  // Normal jitter
    }
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
    
    HeartPoint point;
    point.x = img_x;
    point.y = img_y;
    
    // Color based on distance_factor (how far from curve)
    // Add LOTS of randomness to distance_factor for scattered colors in center
    double color_dist = distance_factor + (unit_dist(gen) - 0.5) * 0.25;
    
    // STRICTLY red-pink and white only: R always highest, B much lower (more red, less purple)
    // More scattered colors in center with lots of white and light red-pink
    if (color_dist < 0.30) {
      // Center - mix of white, light red-pink, and medium red-pink (very scattered)
      double color_choice = unit_dist(gen);
      if (color_choice < 0.4) {
        // 40% bright white
        point.r = 245 + static_cast<unsigned char>(unit_dist(gen) * 10);
        point.g = 235 + static_cast<unsigned char>(unit_dist(gen) * 20);
        point.b = 235 + static_cast<unsigned char>(unit_dist(gen) * 15);
      } else if (color_choice < 0.7) {
        // 30% light red-pink (reduced blue for more red tone)
        point.r = 230 + static_cast<unsigned char>(unit_dist(gen) * 25);
        point.g = 180 + static_cast<unsigned char>(unit_dist(gen) * 50);
        point.b = 180 + static_cast<unsigned char>(unit_dist(gen) * 30);
      } else {
        // 30% medium red-pink (reduced blue significantly)
        point.r = 200 + static_cast<unsigned char>(unit_dist(gen) * 40);
        point.g = 120 + static_cast<unsigned char>(unit_dist(gen) * 60);
        point.b = 120 + static_cast<unsigned char>(unit_dist(gen) * 40);
      }
    } else if (color_dist < 0.55) {
      // Transition zone - light to medium red-pink (R > G, B closer to G)
      point.r = 210 + static_cast<unsigned char>(unit_dist(gen) * 45);
      point.g = 140 + static_cast<unsigned char>(unit_dist(gen) * 70);
      point.b = 140 + static_cast<unsigned char>(unit_dist(gen) * 50);
    } else if (color_dist < 0.80) {
      // Bright white/pink (BRIGHTEST) - nearly equal RGB for white
      point.r = 245 + static_cast<unsigned char>(unit_dist(gen) * 10);
      point.g = 220 + static_cast<unsigned char>(unit_dist(gen) * 35);
      point.b = 220 + static_cast<unsigned char>(unit_dist(gen) * 25);
    } else if (color_dist < 0.98) {
      // Bright red-pink edges (R high, G and B lower, B closer to G for red tone)
      point.r = 240 + static_cast<unsigned char>(unit_dist(gen) * 15);
      point.g = 100 + static_cast<unsigned char>(unit_dist(gen) * 80);
      point.b = 100 + static_cast<unsigned char>(unit_dist(gen) * 60);
    } else if (color_dist < 1.10) {
      // Glow layer 1: Very faint red-pink (semi-transparent effect)
      point.r = 80 + static_cast<unsigned char>(unit_dist(gen) * 40);
      point.g = 20 + static_cast<unsigned char>(unit_dist(gen) * 30);
      point.b = 20 + static_cast<unsigned char>(unit_dist(gen) * 30);
    } else if (color_dist < 1.25) {
      // Glow layer 2: Even fainter red-pink
      point.r = 60 + static_cast<unsigned char>(unit_dist(gen) * 30);
      point.g = 15 + static_cast<unsigned char>(unit_dist(gen) * 20);
      point.b = 15 + static_cast<unsigned char>(unit_dist(gen) * 20);
    } else {
      // Glow layer 3: Barely visible red-pink
      point.r = 40 + static_cast<unsigned char>(unit_dist(gen) * 25);
      point.g = 10 + static_cast<unsigned char>(unit_dist(gen) * 15);
      point.b = 10 + static_cast<unsigned char>(unit_dist(gen) * 15);
    }
    
    points.push_back(point);
  }
}
