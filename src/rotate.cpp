#include "rotate.h"
#include <iostream>


void rotate(
  const std::vector<unsigned char> & input,
  const int width,
  const int height,
  const int num_channels,
  std::vector<unsigned char> & rotated)
{
  rotated.resize(height*width*num_channels);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////

  // e.g., 
  /*
  [ 1 2 3      [ 3 6 9 
    4 5 6   ->   2 5 8
    7 8 9 ]      1 4 7 ]

  1. transpose
  [ 1 2 3     [ 1 4 7
    4 5 6   ->  2 5 8
    7 8 9 ]     3 6 9 ]

  2. reflect across x-axis
  [ 1 4 7     [ 3 6 9 
    2 5 8  ->   2 5 8 
    3 6 9 ]     1 4 7 ]
  */

  std::vector<unsigned char> transposed;
  transposed.resize(height*width*num_channels);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      // move to the correct row in the flattened array
      int row = y * width;

      // move to the correct column in the flattened array
      int new_column = x * height;

      // current column in the flattened array
      int column = x;

      // go through all values in each channel - e.g., if 3-channel, c = 0 = R, c = 1 = G, c = 2 = B
      for (int c = 0; c < num_channels; ++c) {
        // multiply by num_channels to get to the index for the start of each pixel
        transposed[(y + new_column) * num_channels + c] = input[(row + column) * num_channels + c];
      }
    }
  }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int transposed_width = height;

      int transposed_height = width;

      // because indexing starts at 0 instead of 1
      int height_index = transposed_height - 1;

      // new y value after reflection against x-axis
      int new_y = height_index - x;

      // move to the correct row in the flattened array
      int row = x * transposed_width;

      // move to the correct column in the flattened array
      int new_column = new_y * transposed_width;

      // current column in the flattened array
      int column = y;

      // go through all values in each channel - e.g., if 3-channel, c = 0 = R, c = 1 = G, c = 2 = B
      for (int c = 0; c < num_channels; ++c) {
        // multiply by num_channels to get to the index for the start of each pixel
        rotated[(new_column + column) * num_channels + c] = transposed[(row + column) * num_channels + c];
      }
    }
  }
}
