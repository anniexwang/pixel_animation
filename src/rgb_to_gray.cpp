#include "rgb_to_gray.h"

void rgb_to_gray(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  std::vector<unsigned char> & gray)
{
  gray.resize(height*width);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////

  for (int i = 0; i < width * height; ++i) {
    int red = rgb[i * 3];
    int green = rgb[i * 3 + 1];
    int blue = rgb[i * 3 + 2];
    gray[i] = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
  }
}


