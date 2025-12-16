#include "create_gif_from_frames.h"
#include <cstdlib>
#include <sstream>
#include <iostream>

bool create_gif_from_frames(
  const std::string & output_gif_path,
  const std::vector<std::string> & frame_paths,
  const int delay_centiseconds
)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  
  // Check if we have any frames
  if (frame_paths.empty()) {
    std::cerr << "Error: No frame paths provided" << std::endl;
    return false;
  }
  
  // Build the ImageMagick convert command
  std::ostringstream command;
  command << "convert -delay " << delay_centiseconds << " -loop 0";
  
  // Add all frame paths
  for (const auto& frame_path : frame_paths) {
    command << " " << frame_path;
  }
  
  // Add output path
  command << " " << output_gif_path;
  
  // Execute the command
  int result = std::system(command.str().c_str());
  
  if (result != 0) {
    std::cerr << "Error: ImageMagick convert command failed with code " << result << std::endl;
    std::cerr << "Command: " << command.str() << std::endl;
    std::cerr << "Make sure ImageMagick is installed and 'convert' is in your PATH" << std::endl;
    return false;
  }
  
  return true;
}
