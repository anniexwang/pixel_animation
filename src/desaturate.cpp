#include "desaturate.h"
#include "hsv_to_rgb.h"
#include "rgb_to_hsv.h"

void desaturate(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  const double factor,
  std::vector<unsigned char> & desaturated)
{
  desaturated.resize(rgb.size());
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

    // desaturate
    s = s * (1 - factor);

    double new_r;
    double new_g;
    double new_b;
    hsv_to_rgb(h, s, v, new_r, new_g, new_b);

    desaturated[i * 3] = new_r;
    desaturated[i * 3 + 1] = new_g;
    desaturated[i * 3 + 2] = new_b;
  }
}
