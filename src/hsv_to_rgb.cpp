#include "hsv_to_rgb.h"
#include <cmath>
#include <algorithm>

void hsv_to_rgb(
  const double h,
  const double s,
  const double v,
  double & r,
  double & g,
  double & b)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  // r = 0;
  // g = 0;
  // b = 0;
  ////////////////////////////////////////////////////////////////////////////

  /*
    1. C = V * S
    2. X = C * (1 - |(H / 60) mod 2 - 1|)
    3. m = V - C
    4. (R', G', B') = (C, X, 0), if 0 <= H < 60
                      (X, C, 0), if 60 <= H < 120
                      (0, C, X), if 120 <= H < 180
                      (0, X, C), if 180 <= H < 240
                      (X, 0, C), if 240 <= H < 300
                      (C, 0, X), if 300 <= H < 360
    5. (R, G, B) = ((R' + m) * 255, (G' + m) * 255, (B' + m) * 255)

    https://www.rapidtables.com/convert/color/hsv-to-rgb.html
  */

  // step 1
  double c = v * s; 

  // step 2
  double x = c * (1 - std::fabs(std::fmod((h / 60.0), 2) - 1));

  // step 3
  double m = v - c;

  // step 4
  double r_prime = 0;
  double g_prime = 0;
  double b_prime = 0;

  if (0 <= h && h < 60) {
    r_prime = c;
    g_prime = x;
    b_prime = 0;
  } 
  else if (60 <= h && h < 120) {
    r_prime = x;
    g_prime = c;
    b_prime = 0;
  } 
  else if (120 <= h && h < 180) {
    r_prime = 0;
    g_prime = c;
    b_prime = x;
  } 
  else if (180 <= h && h < 240) {
    r_prime = 0;
    g_prime = x;
    b_prime = c;
  } 
  else if (240 <= h && h < 300) {
    r_prime = x;
    g_prime = 0;
    b_prime = c;
  } 
  else if (300 <= h && h < 360) {
    r_prime = c;
    g_prime = 0;
    b_prime = x;
  }

  // step 5
  r = (r_prime + m) * 255;
  g = (g_prime + m) * 255;
  b = (b_prime + m) * 255;

}
