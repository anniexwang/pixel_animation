#include "read_rgba_from_png.h"
#include "rgba_to_rgb.h"
#include "rgb_to_gray.h"
#include "reflect.h"
#include "rotate.h"
#include "write_ppm.h"
#include "over.h"
#include "simulate_bayer_mosaic.h"
#include "desaturate.h"
#include "hue_shift.h"
#include "demosaic.h"
#include "generate_heart_points.h"
#include "write_heart_json.h"
#include "read_heart_json.h"
#include "render_points.h"
#include "calculate_centroid.h"
#include "transform_heart_points.h"
#include "generate_star_points.h"
#include "write_star_json.h"
#include "read_star_json.h"
#include "transform_star_points.h"
#include "create_gif_from_frames.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cmath>
#include <iomanip>
#include <sstream>

int main(int argc, char *argv[])
{
  std::vector<std::string> input_filenames(argv + 1, argv + argc);
  int num_inputs = argc-1;
  if(num_inputs == 0)
  {
    const std::vector<std::string> default_input_file_names = {
      // "../data/zpzp.jpg",
      // "../data/zp.jpg",
      // "../data/zpannie.JPG",
      // "../data/sexyzp.JPG",
      "../data/dog.png",
      "../data/glasses.png",
      "../data/laser-beams.png",
      "../data/sparkles.png"
    };
    input_filenames = default_input_file_names;
    num_inputs = 4;
  }

  // read a RGBA .png
  int width,height;
  std::vector<unsigned char> rgba;
  read_rgba_from_png(input_filenames[0],rgba,width,height);

  // Convert to RGB
  std::vector<unsigned char> rgb;
  rgba_to_rgb(rgba,width,height,rgb);

  // Write to .ppm file format
  write_ppm("rgb.ppm",rgb,width,height,3);

  // Reflection
  std::vector<unsigned char> reflected;
  reflect(rgb,width,height,3,reflected);
  write_ppm("reflected.ppm",reflected,width,height,3);

  // Rotation
  std::vector<unsigned char> rotated;
  rotate(rgb,width,height,3,rotated);
  write_ppm("rotated.ppm",rotated,height,width,3);

  // Convert to gray
  std::vector<unsigned char> gray;
  rgb_to_gray(rgb,width,height,gray);
  write_ppm("gray.ppm",gray,width,height,1);

  // Create fake bayer mosaic image
  std::vector<unsigned char> bayer;
  simulate_bayer_mosaic(rgb,width,height,bayer);
  write_ppm("bayer.ppm",bayer,width,height,1);

  // Demosaic that output
  std::vector<unsigned char> demosaicked;
  demosaic(bayer,width,height,demosaicked);
  write_ppm("demosaicked.ppm",demosaicked,width,height,3);

  // Shift the hue of the image by 180°
  std::vector<unsigned char> shifted;
  hue_shift(rgb,width,height,180.0,shifted);
  write_ppm("shifted.ppm",shifted,width,height,3);

  // Partially desaturate an image by 25%
  std::vector<unsigned char> desaturated;
  desaturate(rgb,width,height,0.25,desaturated);
  write_ppm("desaturated.ppm",desaturated,width,height,3);

  // Alpha composite multiple images (if present)
  std::vector<unsigned char> composite_rgba;
  read_rgba_from_png(input_filenames[0],composite_rgba,width,height);
  for(int f = 1;f<num_inputs;f++)
  {
    std::vector<unsigned char> next_rgba;
    int next_height,next_width;
    read_rgba_from_png(input_filenames[f],next_rgba,next_width,next_height);
    assert(height == next_height && "height must match");
    assert(width == next_width && "width must match");
    over(next_rgba,composite_rgba,width,height,composite_rgba);
  }
  std::vector<unsigned char> composite;
  rgba_to_rgb(composite_rgba,width,height,composite);
  write_ppm("composite.ppm",composite,width,height,3);

  // Heart animation: Generate heart.json if it doesn't exist
  const std::string heart_json_path = "../data/heart.json";
  struct stat buffer;
  bool heart_json_exists = (stat(heart_json_path.c_str(), &buffer) == 0);
  
  std::vector<HeartPoint> heart_points;
  
  if (!heart_json_exists) {
    std::cout << "Generating heart.json..." << std::endl;
    generate_heart_points(heart_points, 500, 500, 5000);  // More particles for denser look
    write_heart_json(heart_json_path, heart_points);
    std::cout << "Generated " << heart_points.size() << " heart points." << std::endl;
  } else {
    std::cout << "heart.json already exists, skipping generation." << std::endl;
  }
  
  // Load points from heart.json
  std::cout << "Loading heart points from " << heart_json_path << "..." << std::endl;
  if (!read_heart_json(heart_json_path, heart_points)) {
    std::cerr << "Error: Failed to read heart.json" << std::endl;
    return 1;
  }
  std::cout << "Loaded " << heart_points.size() << " heart points." << std::endl;
  
  // Render single static frame
  std::cout << "Rendering static heart image..." << std::endl;
  const int heart_width = 500;
  const int heart_height = 500;
  std::vector<unsigned char> heart_image;
  heart_image.resize(heart_width * heart_height * 3);
  
  render_points(heart_image, heart_width, heart_height, heart_points, 1);
  write_ppm("heart_static.ppm", heart_image, heart_width, heart_height, 3);
  std::cout << "Static heart image saved as heart_static.ppm" << std::endl;
  
  // Generate animation frames
  std::cout << "\nGenerating animation frames..." << std::endl;
  
  // Calculate centroid for contraction center
  double center_x, center_y;
  calculate_centroid(heart_points, center_x, center_y);
  std::cout << "Heart centroid: (" << center_x << ", " << center_y << ")" << std::endl;
  
  // Animation parameters
  const int num_frames = 30;
  const double contraction_amplitude = 0.15;
  
  // Generate each frame
  for (int frame = 0; frame < num_frames; frame++) {
    // Calculate contraction factor using cosine wave
    // This creates a smooth pulsing effect
    const double phase = (2.0 * M_PI * frame) / num_frames;
    const double contraction_factor = 1.0 - contraction_amplitude * (1.0 + cos(phase)) / 2.0;
    
    // Transform points based on contraction factor
    std::vector<HeartPoint> transformed_points;
    transform_heart_points(heart_points, transformed_points, center_x, center_y, contraction_factor);
    
    // Render the transformed points
    std::vector<unsigned char> frame_image;
    frame_image.resize(heart_width * heart_height * 3);
    render_points(frame_image, heart_width, heart_height, transformed_points, 1);
    
    // Generate filename with zero-padded frame number
    std::ostringstream filename;
    filename << "heart_frame_" << std::setfill('0') << std::setw(3) << frame << ".ppm";
    
    // Write frame to file
    write_ppm(filename.str(), frame_image, heart_width, heart_height, 3);
    
    // Print progress
    std::cout << "Frame " << frame << "/" << num_frames << " - " 
              << filename.str() << " (contraction: " 
              << std::fixed << std::setprecision(3) << contraction_factor << ")" << std::endl;
  }
  
  std::cout << "\nAnimation complete! Generated " << num_frames << " frames." << std::endl;
  
  // Star animation: Generate star.json if it doesn't exist
  const std::string star_json_path = "../data/star.json";
  bool star_json_exists = (stat(star_json_path.c_str(), &buffer) == 0);
  
  std::vector<StarPoint> star_points;
  
  if (!star_json_exists) {
    std::cout << "\nGenerating star.json..." << std::endl;
    generate_star_points(star_points, 500, 500, 5000);
    write_star_json(star_json_path, star_points);
    std::cout << "Generated " << star_points.size() << " star points." << std::endl;
  } else {
    std::cout << "\nstar.json already exists, skipping generation." << std::endl;
  }
  
  // Load points from star.json
  std::cout << "Loading star points from " << star_json_path << "..." << std::endl;
  if (!read_star_json(star_json_path, star_points)) {
    std::cerr << "Error: Failed to read star.json" << std::endl;
    return 1;
  }
  std::cout << "Loaded " << star_points.size() << " star points." << std::endl;
  
  // Render single static frame
  std::cout << "Rendering static star image..." << std::endl;
  const int star_width = 500;
  const int star_height = 500;
  std::vector<unsigned char> star_image;
  star_image.resize(star_width * star_height * 3);
  
  render_points(star_image, star_width, star_height, star_points, 1);
  write_ppm("star_static.ppm", star_image, star_width, star_height, 3);
  std::cout << "Static star image saved as star_static.ppm" << std::endl;
  
  // Generate animation frames
  std::cout << "\nGenerating star animation frames..." << std::endl;
  
  // Calculate centroid for contraction center
  double star_center_x, star_center_y;
  calculate_centroid(star_points, star_center_x, star_center_y);
  std::cout << "Star centroid: (" << star_center_x << ", " << star_center_y << ")" << std::endl;
  
  // Animation parameters
  const int star_num_frames = 30;
  const double star_contraction_amplitude = 0.15;
  
  // Store frame filenames for GIF creation
  std::vector<std::string> star_frame_paths;
  
  // Generate each frame
  for (int frame = 0; frame < star_num_frames; frame++) {
    // Calculate contraction factor using cosine wave
    // This creates a smooth pulsing effect
    const double phase = (2.0 * M_PI * frame) / star_num_frames;
    const double contraction_factor = 1.0 - star_contraction_amplitude * (1.0 + cos(phase)) / 2.0;
    
    // Transform points based on contraction factor
    std::vector<StarPoint> transformed_star_points;
    transform_star_points(star_points, transformed_star_points, star_center_x, star_center_y, contraction_factor);
    
    // Render the transformed points
    std::vector<unsigned char> frame_image;
    frame_image.resize(star_width * star_height * 3);
    render_points(frame_image, star_width, star_height, transformed_star_points, 1);
    
    // Generate filename with zero-padded frame number
    std::ostringstream filename;
    filename << "star_frame_" << std::setfill('0') << std::setw(3) << frame << ".ppm";
    
    // Write frame to file
    write_ppm(filename.str(), frame_image, star_width, star_height, 3);
    star_frame_paths.push_back(filename.str());
    
    // Print progress
    std::cout << "Frame " << frame << "/" << star_num_frames << " - " 
              << filename.str() << " (contraction: " 
              << std::fixed << std::setprecision(3) << contraction_factor << ")" << std::endl;
  }
  
  std::cout << "\nStar animation complete! Generated " << star_num_frames << " frames." << std::endl;
  
  // Create animated GIF from frames
  std::cout << "\nCreating star_animation.gif..." << std::endl;
  if (create_gif_from_frames("star_animation.gif", star_frame_paths, 3)) {
    std::cout << "Successfully created star_animation.gif" << std::endl;
  } else {
    std::cerr << "Warning: Failed to create star_animation.gif (ImageMagick may not be installed)" << std::endl;
  }
}
