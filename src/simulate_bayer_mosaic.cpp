#include "simulate_bayer_mosaic.h"

void simulate_bayer_mosaic(
  const std::vector<unsigned char> & rgb,
  const int & width,
  const int & height,
  std::vector<unsigned char> & bayer)
{
  bayer.resize(width*height);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  
  // green = even row even column (in terms of index), odd row odd column
  // red = odd row even column
  // blue = even row odd column

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int row = y * width;
      int column = x;

      // even row
      if (y % 2 == 0) { 
        // even column
        if (x % 2 == 0) { 
          // green
          bayer[row + column] = rgb[(row + column) * 3 + 1];
        } 
        // odd column
        else { 
          // blue
          bayer[row + column] = rgb[(row + column) * 3 + 2]; 
        }
      } 
      // odd row
      else { 
        // even column
        if (x % 2 == 0) { 
          // red
          bayer[row + column] = rgb[(row + column) * 3]; 
        } 
        // odd column
        else { 
          // green
          bayer[row + column] = rgb[(row + column) * 3 + 1]; 
        }
      }
    }
  }
}


