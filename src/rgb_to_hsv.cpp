#include "rgb_to_hsv.h"
#include <algorithm>

void rgb_to_hsv(
  const double r,
  const double g,
  const double b,
  double & h,
  double & s,
  double & v)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  // h = 0;
  // s = 0;
  // v = 0;
  ////////////////////////////////////////////////////////////////////////////

  /*
    1.r' = r/255
      g' = g/255
      b' = b/255
    2.cmax = max(r', g', b')
      cmin = min(r', g', b')
      diff = cmax - cmin
    3. Hue calculation :
      h = 0, if diff = 0
        (60 * (((g' - b') / diff) mod 6), if cmax equal r'
        (60 * ((b' - r') / diff) + 2), if cmax equal g'
        (60 * ((r' - g') / diff) + 4), if cmax equal b'
    4. Saturation computation :
      s = 0, if cmax = 0
      s = (diff/cmax), if cmax != 0
    5. Value computation :
      v = cmax

    https://www.rapidtables.com/convert/color/rgb-to-hsv.html
  */

  // step 1
  double r_prime = r / 255.0;
  double g_prime = g / 255.0;
  double b_prime = b / 255.0;

  // step 2
  double cmax = std::max({r_prime, g_prime, b_prime});
  double cmin = std::min({r_prime, g_prime, b_prime});
  double diff = cmax - cmin;

  // step 3 - hue calculation
  if (diff == 0) {
    h = 0;
  } else if (cmax == r_prime) {
    h = 60 * std::fmod(((g_prime - b_prime) / diff), 6.0);
  } else if (cmax == g_prime) {
    h = 60 * ((b_prime - r_prime) / diff + 2);
  } else if (cmax == b_prime) {
    h = 60 * ((r_prime - g_prime) / diff + 4);
  }

  // adjust hue values if they are out of range
  if (h < 0.0) {
    h += 360.0;
  }
  if (h >= 360.0) {
    h -= 360.0;
  }

  // step 4 - saturation computation
  if (cmax == 0) {
    s = 0;
  } else {
    s = (diff / cmax);
  }

  // step 5 - value computation
  v = cmax;
}
