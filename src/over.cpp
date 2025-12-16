#include "over.h"

void over(
  const std::vector<unsigned char> & A,
  const std::vector<unsigned char> & B,
  const int & width,
  const int & height,
  std::vector<unsigned char> & C)
{
  C.resize(A.size());
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////

  /*
  
  1.A_src = alpha_s * (1 - alpha_d)
    A_dst = alpha_d * (1 - alpha_s)
    A_both = alpha_s * alpha_d
    ** note alpha_s and alpha_d is the 4th value in the source and destination for RGBA
  2.alpha channel of result
    A_src * [as] + A_dest * [ad] + A_both * [ab]
    as = 1, ad = 0, ab = 1
  3.A_src * [s] + A_dest * [d] + A_both * [b]
  DestAtop - [s] = s, [d] = 0, [b] = d
  */

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {

      int row = y * width;
      int column = x;

      double alpha_s = B[(row + column) * 4 + 3] / 255.0;
      double alpha_d = A[(row + column) * 4 + 3] / 255.0;

      double A_src = alpha_s * (1.0 - alpha_d);
      double A_dst = alpha_d * (1.0 - alpha_s);
      double A_both = alpha_s * alpha_d;
      
      for (int c = 0; c < 3; ++c) {
        // multiply by num_channels to get to the index for the start of each pixel
        double s = B[(row + column) * 4 + c] / 255.0;
        double d = A[(row + column) * 4 + c] / 255.0;
        
        C[(row + column) * 4 + c] = (A_src * s + A_dst * 0 + A_both * d) * 255.0;
      }
      C[(row + column) * 4 + 3] = alpha_s * 255.0;
    }
  }
}
