# Star Animation Design Document

## Overview

The star animation feature extends the existing raster image processing project by adding a complete star-shaped particle animation system. This system mirrors the architecture of the existing heart animation, providing functions to generate star-shaped particle distributions, serialize/deserialize them to JSON, render them to images, transform them for animation, and combine frames into an animated GIF.

The implementation follows the established project patterns: header files in `include/`, implementations in `src/`, and integration in `main.cpp`. All star functions are named consistently with the heart functions, replacing "heart" with "star" to maintain code clarity and avoid conflicts.

## Architecture

The star animation system consists of five main components:

1. **Star Point Generation** - Creates a five-pointed star distribution of colored particles
2. **JSON Serialization** - Saves and loads star particle data
3. **Point Rendering** - Draws particles to image buffers
4. **Point Transformation** - Scales particles for animation effects
5. **GIF Creation** - Combines animation frames into an animated GIF file

These components work together in a pipeline:
```
Generate Star Points → Save to star.json → Load from star.json → 
Transform Points (per frame) → Render to PPM → Convert PPMs to GIF
```

The system reuses existing functions where possible:
- `calculate_centroid()` - Already works with any point structure
- `render_points()` - Can render any point type with x, y, r, g, b fields
- `write_ppm()` - Existing PPM writer

## Components and Interfaces

### 1. StarPoint Structure

```cpp
struct StarPoint {
  double x;           // X coordinate
  double y;           // Y coordinate  
  unsigned char r;    // Red channel [0-255]
  unsigned char g;    // Green channel [0-255]
  unsigned char b;    // Blue channel [0-255]
};
```

This structure is identical to `HeartPoint` but maintains separate naming for clarity.

### 2. Star Point Generation

**Header:** `include/generate_star_points.h`
**Implementation:** `src/generate_star_points.cpp`

```cpp
void generate_star_points(
  std::vector<StarPoint> & points,
  const int image_width,
  const int image_height,
  const int num_points
);
```

Generates a five-pointed star using parametric equations. The star is created by:
- Sampling points along a five-pointed star curve
- Applying random distance offsets for particle scatter
- Assigning colors based on distance from outline (gradient effect)
- Adding jitter to eliminate visible patterns
- Centering the star in the image canvas

**Five-Pointed Star Parametric Equations:**
```
For angle θ from 0 to 2π:
- Radius alternates between outer_radius and inner_radius at 5 points
- x = radius(θ) * cos(θ)
- y = radius(θ) * sin(θ)
```

### 3. JSON Serialization

**Write Header:** `include/write_star_json.h`
**Write Implementation:** `src/write_star_json.cpp`

```cpp
bool write_star_json(
  const std::string & filename,
  const std::vector<StarPoint> & points
);
```

**Read Header:** `include/read_star_json.h`
**Read Implementation:** `src/read_star_json.cpp`

```cpp
bool read_star_json(
  const std::string & filename,
  std::vector<StarPoint> & points
);
```

Uses the existing `json.hpp` library to serialize/deserialize star points. JSON format:
```json
{
  "points": [
    {"x": 250.5, "y": 100.2, "r": 255, "g": 200, "b": 50},
    ...
  ]
}
```

### 4. Point Transformation

**Header:** `include/transform_star_points.h`
**Implementation:** `src/transform_star_points.cpp`

```cpp
void transform_star_points(
  const std::vector<StarPoint> & input_points,
  std::vector<StarPoint> & output_points,
  const double center_x,
  const double center_y,
  const double scale_factor
);
```

Transforms star points by scaling them toward/away from a center point. Used for contraction animation.

### 5. GIF Creation

**Header:** `include/create_gif_from_frames.h`
**Implementation:** `src/create_gif_from_frames.cpp`

```cpp
bool create_gif_from_frames(
  const std::string & output_gif_path,
  const std::vector<std::string> & frame_paths,
  const int delay_centiseconds
);
```

Converts a sequence of PPM frames into an animated GIF using ImageMagick's `convert` command via system call.

## Data Models

### StarPoint
- `x`, `y`: Double precision coordinates in image space
- `r`, `g`, `b`: 8-bit color channels

### Animation Frame
- Image buffer: `std::vector<unsigned char>` of size `width * height * 3`
- RGB format, row-major order
- Each frame represents one timestep in the animation

## Correctness Properties

*A property is a characteristic or behavior that should hold true across all valid executions of a system-essentially, a formal statement about what the system should do. Properties serve as the bridge between human-readable specifications and machine-verifiable correctness guarantees.*


### Property Reflection

After analyzing the acceptance criteria, several properties can be consolidated:
- Properties 2.1 and 2.2 both test serialization and can be combined into a single round-trip property
- Properties about file naming and existence (2.3, 2.4, 4.4, 5.2, 5.3, 5.4) are integration tests rather than core correctness properties
- Edge case 3.3 will be handled by the property test generator creating boundary-case inputs

### Core Correctness Properties

Property 1: Star particle generation produces valid coordinates
*For any* image dimensions and particle count, all generated star particles should have coordinates within the image bounds (0 ≤ x < width, 0 ≤ y < height)
**Validates: Requirements 1.5**

Property 2: Star particles follow distance-based color gradient
*For any* generated star distribution, particles closer to the star outline should have different color characteristics than particles near the center, creating a gradient effect
**Validates: Requirements 1.3**

Property 3: JSON serialization round-trip preserves data
*For any* star particle distribution, writing to JSON and reading back should produce an equivalent set of particles with identical positions and colors
**Validates: Requirements 2.1, 2.2**

Property 4: Rendering places particles at correct positions
*For any* star particle with coordinates (x, y) and color (r, g, b), the rendered image should contain that color at or near position (x, y)
**Validates: Requirements 3.1**

Property 5: Particle radius affects rendered output
*For any* star particle distribution, rendering with radius R1 should produce a different image than rendering with radius R2 when R1 ≠ R2
**Validates: Requirements 3.2**

Property 6: Centroid calculation finds geometric center
*For any* star particle distribution, the calculated centroid should be the average of all particle x-coordinates and y-coordinates
**Validates: Requirements 4.1**

Property 7: Transformation scales particles relative to center
*For any* star particle at position (x, y), center point (cx, cy), and scale factor s, the transformed particle should be at position (cx + s*(x-cx), cy + s*(y-cy))
**Validates: Requirements 4.2**

Property 8: Animation uses cosine wave for contraction
*For any* frame number f in a 30-frame animation, the contraction factor should follow the formula: 1.0 - amplitude * (1.0 + cos(2π*f/30)) / 2.0
**Validates: Requirements 4.3**

## Error Handling

### File I/O Errors
- JSON read/write functions return `bool` to indicate success/failure
- Caller should check return values and handle errors appropriately
- Invalid JSON format should be detected and reported

### Boundary Conditions
- Particles outside image bounds during generation are skipped (regenerate that particle)
- Particles outside bounds during rendering are clipped (no drawing occurs)
- Zero or negative dimensions should be handled gracefully (return empty result or error)

### Memory Allocation
- Use `std::vector` for automatic memory management
- Reserve capacity when particle count is known to avoid reallocations
- Clear vectors before populating to ensure clean state

### GIF Creation Errors
- Check if ImageMagick `convert` command is available
- Return `false` if GIF creation fails
- Provide error messages to stderr for debugging

## Testing Strategy

### Unit Testing
The implementation will include unit tests for:
- Individual star point generation with known parameters
- JSON serialization with sample data
- Transformation calculations with specific inputs
- Centroid calculation with known point sets
- GIF creation with a small set of test frames

### Property-Based Testing
The implementation will use property-based testing to verify universal properties across many random inputs. We will use the **Catch2** framework with its built-in property testing support (GENERATE macro for parameterized tests).

Each property-based test will:
- Run a minimum of 100 iterations with random inputs
- Be tagged with a comment referencing the design document property
- Use the format: `// Feature: star-animation, Property N: <property text>`

Property tests will cover:
- **Property 1**: Generate stars with random dimensions and verify all coordinates are in bounds
- **Property 2**: Generate stars and verify color gradient exists (measure color variance by distance)
- **Property 3**: Round-trip test with random star distributions
- **Property 4**: Render random particles and verify colors appear at expected positions
- **Property 5**: Render with different radii and verify outputs differ
- **Property 6**: Calculate centroid of random point sets and verify it matches manual calculation
- **Property 7**: Transform random particles and verify new positions match formula
- **Property 8**: Generate all 30 frames and verify contraction factors follow cosine wave

### Integration Testing
Integration tests will verify:
- Complete pipeline from generation → JSON → rendering → GIF
- Coexistence with heart animation (both run without conflicts)
- File naming conventions (star files don't overwrite heart files)

## Implementation Notes

### Five-Pointed Star Generation
The star will use a parametric approach similar to the heart:
```cpp
for (int i = 0; i < num_points; i++) {
  double angle = random(0, 2*PI);
  
  // Determine if we're at a star point (outer) or valley (inner)
  double angle_in_segment = fmod(angle, 2*PI/5);
  double radius = lerp(inner_radius, outer_radius, 
                       triangle_wave(angle_in_segment));
  
  double x = radius * cos(angle);
  double y = radius * sin(angle);
  
  // Apply distance factor and jitter...
}
```

### Color Gradient
Colors will transition from bright yellow/white at edges to darker orange/red toward center:
- Outer edge (distance > 0.8): Bright yellow/white (R=255, G=240-255, B=100-200)
- Mid range (0.4 < distance < 0.8): Yellow-orange (R=255, G=180-220, B=50-100)
- Center (distance < 0.4): Orange-red (R=200-255, G=100-150, B=20-50)

### GIF Creation
Use ImageMagick's `convert` command:
```bash
convert -delay 3 -loop 0 star_frame_*.ppm star_animation.gif
```
- `-delay 3`: 3 centiseconds (30ms) per frame = ~33 fps
- `-loop 0`: Infinite loop
- Input: All star_frame_*.ppm files
- Output: star_animation.gif

### Performance Considerations
- Pre-allocate vectors with known sizes
- Use fixed random seed for reproducible generation (optional)
- Render particles in a single pass over the image buffer
- Consider particle radius when determining affected pixels
