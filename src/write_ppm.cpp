#include "write_ppm.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_ppm(
  const std::string & filename,
  const std::vector<unsigned char> & data,
  const int width,
  const int height,
  const int num_channels)
{
  assert(
    (num_channels == 3 || num_channels == 1 ) &&
    ".ppm only supports RGB or grayscale images");
    ////////////////////////////////////////////////////////////////////////////
    // Replace with your code here:
    // return false;
    ////////////////////////////////////////////////////////////////////////////


  std::ofstream ofs(filename);
    if (!ofs) return false;

    // Write header
    if (num_channels == 1) {
        ofs << "P2\n"; // ASCII grayscale
    } else {
        ofs << "P3\n"; // ASCII RGB
    }
    ofs << width << " " << height << "\n255\n";

    // Write pixels
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * num_channels;

            if (num_channels == 1) {
                ofs << static_cast<int>(data[idx]) << " ";
            } else {
                ofs << static_cast<int>(data[idx])     << " "  // R
                    << static_cast<int>(data[idx + 1]) << " "  // G
                    << static_cast<int>(data[idx + 2]) << " "; // B
            }
        }
        ofs << "\n";
    }

    return ofs.good();
  
}
