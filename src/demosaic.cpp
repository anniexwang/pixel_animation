#include "demosaic.h"
#include <iostream>
#include <algorithm>
#include <cmath>


void demosaic(
  const std::vector<unsigned char> & bayer,
  const int & width,
  const int & height,
  std::vector<unsigned char> & rgb)
{
  rgb.resize(width*height*3);
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

      int up = (y > 0) ? bayer[row - width + column] : 0;
      int down = (y < height - 1) ? bayer[row + width + column] : 0;
      int left = (x > 0) ? bayer[row + column - 1] : 0;
      int right = (x < width - 1) ? bayer[row + column + 1] : 0;
      int upleft = (y > 0 && x > 0) ? bayer[row - width + column - 1] : 0;
      int upright = (y > 0 && x < width - 1) ? bayer[row - width + column + 1] : 0;
      int downleft = (y < height - 1 && x > 0) ? bayer[row + width + column - 1] : 0;
      int downright = (y < height - 1 && x < width - 1) ? bayer[row + width + column + 1] : 0;

      // even row
      if (y % 2 == 0) { 
        // even column
        if (x % 2 == 0) { 
          // green
          rgb[(row + column) * 3 + 1] = bayer[row + column];
          
          // red = average(up, down)
          std::vector<int> red_values = {up, down};
          int total_red = std::count_if(red_values.begin(), red_values.end(), [](int x) { return x != 0; });
          rgb[(row + column) * 3] = static_cast<unsigned char>(std::floor(static_cast<double>(up + down)/ total_red));
          // blue = average(left, right)
          std::vector<int> blue_values = {left, right};
          int total_blue = std::count_if(blue_values.begin(), blue_values.end(), [](int x) { return x != 0; });
          rgb[(row + column) * 3 + 2] = static_cast<unsigned char>(std::floor(static_cast<double>(left + right) / total_blue));
        } 
        // odd column
        else { 
          // blue
          rgb[(row + column) * 3 + 2] = bayer[row + column]; 

          // red = average(diagonals)
          std::vector<int> red_values = {upleft, upright, downleft, downright};
          int total_red = std::count_if(red_values.begin(), red_values.end(), [](int x) { return x != 0; });
          rgb[(row + column) * 3] = static_cast<unsigned char>(std::floor(static_cast<double>(upleft + upright + downleft + downright) / total_red));
          // green = average(up, down, left, right)
          std::vector<int> green_values = {up, down, left, right};
          int total_green = std::count_if(green_values.begin(), green_values.end(), [](int x) { return x != 0; });
          rgb[(row + column) * 3 + 1] = static_cast<unsigned char>(std::floor(static_cast<double>(up + down + left + right) / total_green));
        }
      } 
      // odd row
      else { 
        // even column
        if (x % 2 == 0) { 
          // red
          rgb[(row + column) * 3] = bayer[row + column]; 

          // green = average(up, down, left, right)
          std::vector<int> green_values = {up, down, left, right};
          int total_green = std::count_if(green_values.begin(), green_values.end(), [](int x) { return x != 0; });
          rgb[(row + column) * 3 + 1] = static_cast<unsigned char>(std::floor(static_cast<double>(up + down + left + right) / total_green));
          // blue = average(diagonals)
          std::vector<int> blue_values = {upleft, upright, downleft, downright};
          int total_blue = std::count_if(blue_values.begin(), blue_values.end(), [](int x) { return x != 0; });
          rgb[(row + column) * 3 + 2] = static_cast<unsigned char>(std::floor(static_cast<double>(upleft + upright + downleft + downright) / total_blue));
        } 
        // odd column
        else { 
          // green
          rgb[(row + column) * 3 + 1] = bayer[row + column]; 

          // red = average(left, right)
          std::vector<int> red_values = {left, right};
          int total_red = std::count_if(red_values.begin(), red_values.end(), [](int x) { return x != 0; });
          rgb[(row + column) * 3] = static_cast<unsigned char>(std::floor(static_cast<double>(left + right) / total_red));
          // blue = average(up, down)
          std::vector<int> blue_values = {up, down};
          int total_blue = std::count_if(blue_values.begin(), blue_values.end(), [](int x) { return x != 0; });
          rgb[(row + column) * 3 + 2] = static_cast<unsigned char>(std::floor(static_cast<double>(up + down) / total_blue));
        }
      }
    }
  }
}
