#include "reflect.h"
#include <iostream>

void reflect(
  const std::vector<unsigned char> & input,
  const int width,
  const int height,
  const int num_channels,
  std::vector<unsigned char> & reflected)
{
  reflected.resize(width*height*num_channels);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////

  std::cout << num_channels << std::endl;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      // because indexing starts at 0 instead of 1
      int width_index = width - 1;

      // new x value after reflection against y-axis
      int new_x = width_index - x;

      // move to the correct row in the flattened array
      int row = y * width;

      // current column in the flattened array
      int column = x;

      // go through all values in each channel - e.g., if 3-channel, c = 0 = R, c = 1 = G, c = 2 = B
      for (int c = 0; c < num_channels; ++c) {
        // multiply by num_channels to get to the index for the start of each pixel
        reflected[(row + new_x) * num_channels + c] = input[(row + column) * num_channels + c];
      }
    }
  }
}
