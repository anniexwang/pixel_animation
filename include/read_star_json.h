#ifndef READ_STAR_JSON_H
#define READ_STAR_JSON_H

#include <string>
#include <vector>
#include "generate_star_points.h"

// Read star points from a JSON file
//
// Inputs:
//   filename  path to input JSON file as string
//   points  output vector to store loaded star points
//
// Expected JSON format:
//   {
//     "points": [
//       {"x": 250.5, "y": 300.2, "r": 255, "g": 100, "b": 150},
//       ...
//     ]
//   }
//
// Returns true on success, false on failure (e.g., file doesn't exist,
// malformed JSON, missing required fields, invalid RGB values)
bool read_star_json(
  const std::string & filename,
  std::vector<StarPoint> & points
);

#endif
