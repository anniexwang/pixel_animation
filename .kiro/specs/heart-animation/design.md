# Heart Animation Design Document

## Overview

This design extends the existing raster image processing system to support particle-based heart animation. The system will generate a heart.json file containing colored point coordinates, then create a sequence of PPM frames showing a pulsing heart effect through geometric transformation of the points.

The animation achieves the pulsing effect by applying a time-varying contraction factor that scales point positions toward the heart's center, creating smooth expansion and contraction cycles.

## Architecture

### High-Level Flow

```
1. Check if data/heart.json exists
   ├─ No: Generate heart.json with ~2000-3000 points
   └─ Yes: Load existing heart.json

2. Parse JSON and extract points with colors

3. Calculate heart centroid (center point)

4. For each frame (0 to N-1):
   ├─ Calculate contraction factor using sine wave
   ├─ Transform all points toward/away from center
   ├─ Render points to image buffer
   └─ Write frame as PPM file

5. Output: heart_frame_000.ppm, heart_frame_001.ppm, ...
```

### Integration with Existing System

The heart animation will integrate with the existing codebase by:
- Using the existing `write_ppm` function for frame output
- Using the existing `json.hpp` library for JSON parsing
- Following the header/source file pattern (include/ and src/)
- Adding new functions to main.cpp to orchestrate the animation
- Maintaining compatibility with the existing CMake build system

## Components and Interfaces

### 1. Heart Point Generator

**Purpose**: Generate heart-shaped point distribution with color gradients

**Header**: `include/generate_heart_points.h`

```cpp
struct HeartPoint {
  double x;
  double y;
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

void generate_heart_points(
  std::vector<HeartPoint> & points,
  const int image_width,
  const int image_height,
  const int num_points
);
```

**Algorithm**:
- Use parametric heart curve equation: 
  - x(t) = 16 * sin³(t)
  - y(t) = 13 * cos(t) - 5 * cos(2t) - 2 * cos(3t) - cos(4t)
- Sample points along the curve and add random offsets for particle effect
- Create denser distribution at edges, sparser in center
- Assign colors based on distance from center:
  - Center: bright pink/white (255, 200, 220)
  - Mid: bright pink (255, 100, 150)
  - Edge: dark red/pink (180, 40, 80)

### 2. JSON Writer

**Purpose**: Write heart points to JSON file

**Header**: `include/write_heart_json.h`

```cpp
void write_heart_json(
  const std::string & filename,
  const std::vector<HeartPoint> & points
);
```

**Format**:
```json
{
  "points": [
    {"x": 250.5, "y": 300.2, "r": 255, "g": 100, "b": 150},
    {"x": 251.3, "y": 301.8, "r": 240, "g": 90, "b": 140},
    ...
  ]
}
```

### 3. JSON Reader

**Purpose**: Parse heart.json and load points

**Header**: `include/read_heart_json.h`

```cpp
bool read_heart_json(
  const std::string & filename,
  std::vector<HeartPoint> & points
);
```

**Error Handling**:
- Return false if file doesn't exist or is malformed
- Validate that all required fields (x, y, r, g, b) are present
- Validate RGB values are in range [0, 255]

### 4. Point Transformer

**Purpose**: Apply contraction transformation to points

**Header**: `include/transform_heart_points.h`

```cpp
void transform_heart_points(
  const std::vector<HeartPoint> & original_points,
  std::vector<HeartPoint> & transformed_points,
  const double center_x,
  const double center_y,
  const double contraction_factor
);
```

**Transformation**:
- For each point (x, y):
  - dx = x - center_x
  - dy = y - center_y
  - new_x = center_x + dx * contraction_factor
  - new_y = center_y + dy * contraction_factor
- Contraction factor ranges from 0.85 (contracted) to 1.0 (expanded)

### 5. Point Renderer

**Purpose**: Render points to image buffer

**Header**: `include/render_points.h`

```cpp
void render_points(
  std::vector<unsigned char> & image,
  const int width,
  const int height,
  const std::vector<HeartPoint> & points,
  const int point_radius
);
```

**Rendering**:
- Initialize image with background color (40, 20, 60) - dark purple
- For each point:
  - If point_radius == 1: draw single pixel
  - If point_radius > 1: draw filled circle using midpoint circle algorithm
  - Skip points outside image bounds
  - Use simple overwrite (no blending) for performance

### 6. Centroid Calculator

**Purpose**: Calculate geometric center of points

**Header**: `include/calculate_centroid.h`

```cpp
void calculate_centroid(
  const std::vector<HeartPoint> & points,
  double & center_x,
  double & center_y
);
```

**Algorithm**:
- center_x = sum(all x coordinates) / num_points
- center_y = sum(all y coordinates) / num_points

## Data Models

### HeartPoint Structure

```cpp
struct HeartPoint {
  double x;           // X coordinate (can be fractional for smooth animation)
  double y;           // Y coordinate (can be fractional for smooth animation)
  unsigned char r;    // Red channel [0-255]
  unsigned char g;    // Green channel [0-255]
  unsigned char b;    // Blue channel [0-255]
};
```

### Animation Parameters

```cpp
struct AnimationConfig {
  int num_frames = 30;              // Total frames in animation
  int image_width = 500;            // Output image width
  int image_height = 500;           // Output image height
  int point_radius = 1;             // Radius of each point (1 = single pixel)
  double contraction_amplitude = 0.15;  // How much the heart contracts (0.15 = 15%)
  unsigned char bg_r = 40;          // Background red
  unsigned char bg_g = 20;          // Background green
  unsigned char bg_b = 60;          // Background blue
};
```

## 

## Error Handling

### File I/O Errors
- If heart.json cannot be created: print error message and exit with code 1
- If heart.json exists but cannot be read: print error message and exit with code 1
- If output directory is not writable: print error message and exit with code 1

### JSON Parsing Errors
- If JSON is malformed: print specific parsing error and exit with code 1
- If required fields are missing: print which fields are missing and exit with code 1
- If RGB values are out of range: clamp to [0, 255] and print warning

### Runtime Errors
- If no points are loaded: print error and exit with code 1
- If image dimensions are invalid (≤ 0): print error and exit with code 1
- If frame generation fails: print error for that frame but continue with remaining frames

## Testing Strategy

### Unit Tests

Unit tests will verify specific examples and edge cases:

1. **JSON Generation Test**: Verify heart.json is created with valid structure
2. **JSON Parsing Test**: Verify points are correctly loaded from valid JSON
3. **Centroid Calculation Test**: Verify centroid is calculated correctly for known point sets
4. **Transformation Test**: Verify point transformation with known contraction factors
5. **Boundary Test**: Verify points outside image bounds are handled correctly
6. **Color Validation Test**: Verify RGB values are clamped to [0, 255]

### Property-Based Tests

Property-based tests will verify universal properties across all inputs using a C++ property testing library (e.g., RapidCheck or CppQuickCheck):


## Correctness Properties

*A property is a characteristic or behavior that should hold true across all valid executions of a system-essentially, a formal statement about what the system should do. Properties serve as the bridge between human-readable specifications and machine-verifiable correctness guarantees.*

### Property Reflection

After analyzing all acceptance criteria, I identified the following redundancies:
- Properties 1.5 and 1.6 both relate to point distribution and can be tested together
- Property 2.1 is subsumed by Property 2.2 (if points move toward center, contraction is applied)
- Property 3.2 is redundant with Property 3.5 (both test contraction factor distribution)
- Property 4.1 is implicitly tested by other rendering properties
- Property 5.3 is implicitly tested by Property 5.1
- Property 7.2 is the specific algorithm for 7.1, so they can be combined

### Core Properties

Property 1: JSON round-trip preservation
*For any* set of heart points, writing them to JSON and reading them back should produce equivalent point data (same coordinates and colors within floating-point tolerance)
**Validates: Requirements 1.7, 1.8**

Property 2: Color gradient correctness
*For any* pair of points in the generated heart, the point closer to the centroid should have equal or brighter color intensity (sum of R+G+B) than the point farther from the centroid
**Validates: Requirements 1.5**

Property 3: Invalid JSON error handling
*For any* malformed JSON input (missing fields, invalid types, out-of-range values), the parser should report an error and not crash
**Validates: Requirements 1.9**

Property 4: Contraction moves points toward center
*For any* set of points and contraction factor less than 1.0, every transformed point should be closer to (or at the same distance from) the center than its original position
**Validates: Requirements 2.2**

Property 5: Contraction factor periodicity
*For any* animation sequence, the contraction factors across frames should follow a smooth periodic function where the last frame's factor is close to the first frame's factor (within 5% for seamless looping)
**Validates: Requirements 2.3, 3.4**

Property 6: Topology preservation under transformation
*For any* two points A and B, if A is to the left of B before transformation, A should remain to the left of B after transformation (preserves relative spatial relationships)
**Validates: Requirements 2.4**

Property 7: Identity transformation
*For any* set of points, applying a transformation with contraction factor 1.0 should return points at their original positions (within floating-point tolerance)
**Validates: Requirements 2.5**

Property 8: Frame count matches request
*For any* requested number of frames N (where N > 0), the system should generate exactly N output files
**Validates: Requirements 3.1**

Property 9: Sequential frame numbering
*For any* animation sequence, frame filenames should be numbered sequentially starting from 000 with no gaps
**Validates: Requirements 3.3**

Property 10: Uniform contraction distribution
*For any* animation sequence with N frames, the difference in contraction factor between consecutive frames should be roughly uniform (standard deviation < 10% of mean difference)
**Validates: Requirements 3.5**

Property 11: Out-of-bounds points are skipped
*For any* point with coordinates outside the image bounds [0, width) × [0, height), that point should not cause an error and should not appear in the rendered output
**Validates: Requirements 4.2**

Property 12: Point radius affects rendered size
*For any* point radius R > 1, the rendered point should occupy approximately π × R² pixels (within 20% tolerance for discrete pixel approximation)
**Validates: Requirements 4.5**

Property 13: Background color initialization
*For any* frame, all pixels that are not covered by rendered points should have the specified background color
**Validates: Requirements 5.1**

Property 14: RGB value validation
*For any* background color specification, RGB values should be clamped to the range [0, 255] if they fall outside this range
**Validates: Requirements 5.4**

Property 15: Centroid calculation correctness
*For any* set of N points, the calculated centroid should equal (sum of all x coordinates / N, sum of all y coordinates / N)
**Validates: Requirements 7.1, 7.2**

Property 16: Centroid as contraction center
*For any* contraction operation without manual center override, the center point used should equal the calculated centroid of the original points
**Validates: Requirements 7.3**


## Implementation Notes

### Heart Shape Generation

The parametric equations for the heart curve are:
```
x(t) = 16 * sin³(t)
y(t) = 13 * cos(t) - 5 * cos(2t) - 2 * cos(3t) - cos(4t)
where t ∈ [0, 2π]
```

To create the particle effect:
1. Sample 500-800 points along the curve (varying t)
2. For each curve point, generate 3-5 nearby points with random offsets
3. Add additional random points in the interior (sparser density)
4. Total: approximately 2000-3000 points

### Color Assignment Algorithm

```cpp
for each point (x, y):
  distance = sqrt((x - center_x)² + (y - center_y)²)
  normalized_distance = distance / max_distance  // [0, 1]
  
  if normalized_distance < 0.3:  // Center region
    r = 255
    g = 200 + random(-20, 20)
    b = 220 + random(-20, 20)
  else if normalized_distance < 0.7:  // Mid region
    r = 255
    g = 100 + random(-20, 20)
    b = 150 + random(-20, 20)
  else:  // Edge region
    r = 180 + random(-20, 20)
    g = 40 + random(-20, 20)
    b = 80 + random(-20, 20)
```

### Contraction Factor Calculation

```cpp
double calculate_contraction_factor(int frame, int total_frames, double amplitude) {
  double phase = (2.0 * M_PI * frame) / total_frames;
  double factor = 1.0 - amplitude * (1.0 + cos(phase)) / 2.0;
  return factor;  // Ranges from (1.0 - amplitude) to 1.0
}
```

This creates a smooth pulsing effect where:
- Frame 0: factor = 1.0 - amplitude (most contracted)
- Frame N/2: factor = 1.0 (fully expanded)
- Frame N: factor ≈ 1.0 - amplitude (back to contracted, loops smoothly)

### Performance Considerations

- Use `std::vector::reserve()` to pre-allocate memory for points
- Render points in a single pass over the point list
- Use integer arithmetic for pixel coordinates where possible
- Consider using OpenMP for parallel frame generation (optional enhancement)

### File Naming Convention

Frames will be named: `heart_frame_000.ppm`, `heart_frame_001.ppm`, etc.
- Zero-padded to 3 digits (supports up to 999 frames)
- Consistent with existing PPM output naming

## Dependencies

- Existing: `write_ppm` function from the current codebase
- Existing: `json.hpp` library (already in json/ directory)
- Standard library: `<cmath>`, `<vector>`, `<string>`, `<fstream>`, `<random>`
- No new external dependencies required

## Future Enhancements (Out of Scope)

- Support for multiple heart shapes or custom shapes
- Interactive parameter adjustment
- Video file output (MP4, GIF)
- Real-time preview window
- Particle physics (velocity, acceleration)
- Multiple animation patterns (pulse, rotate, wave)
