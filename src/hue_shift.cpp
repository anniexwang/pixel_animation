#include "hue_shift.h"
#include "hsv_to_rgb.h"
#include "rgb_to_hsv.h"

void hue_shift(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  const double shift,
  std::vector<unsigned char> & shifted)
{
  shifted.resize(rgb.size());
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////

  for (int i = 0; i < width * height; ++i) {
    int r = rgb[i * 3];
    int g = rgb[i * 3 + 1];
    int b = rgb[i * 3 + 2];

    double h;
    double s;
    double v;
    rgb_to_hsv(r, g, b, h, s, v);

    // shift hue
    h = std::fmod((h + shift), 360.0);

    // adjust hue values if they are out of range
    if (h < 0.0) {
      h += 360.0;
    }
    if (h >= 360.0) {
      h -= 360.0;
    }

    double new_r;
    double new_g;
    double new_b;
    hsv_to_rgb(h, s, v, new_r, new_g, new_b);

    shifted[i * 3] = new_r;
    shifted[i * 3 + 1] = new_g;
    shifted[i * 3 + 2] = new_b;
  }
}
