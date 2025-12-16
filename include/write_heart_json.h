#ifndef WRITE_HEART_JSON_H
#define WRITE_HEART_JSON_H

#include <string>
#include <vector>
#include "generate_heart_points.h"

// Write heart points to a JSON file
//
// Inputs:
//   filename  path to output JSON file as string
//   points  vector of HeartPoint structures to write
//
// Output format:
//   {
//     "points": [
//       {"x": 250.5, "y": 300.2, "r": 255, "g": 100, "b": 150},
//       ...
//     ]
//   }
//
// Returns true on success, false on failure (e.g., can't open file)
bool write_heart_json(
  const std::string & filename,
  const std::vector<HeartPoint> & points
);

#endif
