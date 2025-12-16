#ifndef CREATE_GIF_FROM_FRAMES_H
#define CREATE_GIF_FROM_FRAMES_H

#include <string>
#include <vector>

// Creates an animated GIF from a sequence of PPM frame files using ImageMagick
//
// Inputs:
//   output_gif_path  path to the output GIF file
//   frame_paths      vector of paths to input frame files (PPM format)
//   delay_centiseconds  delay between frames in centiseconds (default 3)
// Returns true if GIF creation succeeded, false otherwise
bool create_gif_from_frames(
  const std::string & output_gif_path,
  const std::vector<std::string> & frame_paths,
  const int delay_centiseconds = 3
);

#endif
