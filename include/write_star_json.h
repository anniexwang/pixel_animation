#ifndef WRITE_STAR_JSON_H
#define WRITE_STAR_JSON_H

#include <string>
#include <vector>
#include "generate_star_points.h"

// Write star points to a JSON file
//
// Inputs:
//   filename  path to output JSON file as string
//   points  vector of StarPoint structures to write
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
bool write_star_json(
  const std::string & filename,
  const std::vector<StarPoint> & points
);

#endif
