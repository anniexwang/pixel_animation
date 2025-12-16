#ifndef READ_HEART_JSON_H
#define READ_HEART_JSON_H

#include <string>
#include <vector>
#include "generate_heart_points.h"

// Read heart points from a JSON file
//
// Inputs:
//   filename  path to input JSON file as string
//   points  output vector to store loaded heart points
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
bool read_heart_json(
  const std::string & filename,
  std::vector<HeartPoint> & points
);

#endif
