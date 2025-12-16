# Heart & Star Particle Animation System

A procedural animation system that generates pulsing heart and star effects using particle-based rendering and parametric shape generation.

## 🎬 Animation Preview

![Star Animation](build/star_animation.gif)

*Pulsing star animation with 30 frames showing smooth contraction and expansion*

## ✨ Features

- **Procedural Shape Generation**: Heart and star shapes created using parametric equations
- **Particle System**: Thousands of colored particles with gradient effects
- **Smooth Animation**: Cosine wave-based pulsing with configurable amplitude
- **Multiple Output Formats**: Individual PPM frames + animated GIF
- **JSON Persistence**: Save and load particle distributions for consistency
- **Property-Based Testing**: Mathematical validation of animation properties

## 🚀 Quick Start

### Prerequisites

- **C++20** compatible compiler
- **CMake** ≥ 3.20
- **ImageMagick** (optional, for GIF creation)

### Build & Run

```bash
# Clone and build
git clone --recursive <repository-url>
cd pixel_animation
mkdir build && cd build
cmake ..
make

# Generate animations
./raster
```

### Output Files

The program generates:

**Heart Animation:**
- `heart_static.ppm` - Single frame rendering
- `heart_frame_000.ppm` to `heart_frame_029.ppm` - 30 animation frames
- `../data/heart.json` - Particle distribution data

**Star Animation:**
- `star_static.ppm` - Single frame rendering  
- `star_frame_000.ppm` to `star_frame_029.ppm` - 30 animation frames
- `star_animation.gif` - Animated GIF (requires ImageMagick)
- `../data/star.json` - Particle distribution data

## 🎨 Animation System

### Heart Animation

The heart shape is generated using parametric equations:
```
x(t) = 16 * sin³(t)
y(t) = 13 * cos(t) - 5 * cos(2t) - 2 * cos(3t) - cos(4t)
```

**Features:**
- **Color Gradient**: Bright pink/white (center) → dark red/pink (edges)
- **Particle Distribution**: Denser at edges, sparser in center
- **Pulsing Effect**: Smooth contraction/expansion using cosine waves

### Star Animation

The five-pointed star uses alternating radius values:
```
For angle θ ∈ [0, 2π]:
- Radius alternates between outer_radius and inner_radius
- x = radius(θ) * cos(θ)
- y = radius(θ) * sin(θ)
```

**Features:**
- **Color Gradient**: Bright yellow/white (edges) → orange/red (center)
- **Sparkle Effect**: Random particle offsets for organic appearance
- **GIF Export**: Automatic animated GIF creation

### Animation Mathematics

Both animations use cosine wave functions for smooth pulsing:

```cpp
// Contraction factor calculation
double phase = (2.0 * M_PI * frame) / total_frames;
double contraction_factor = 1.0 - amplitude * (1.0 + cos(phase)) / 2.0;
```

This creates:
- **Frame 0**: Most contracted (factor = 1.0 - amplitude)
- **Frame N/2**: Fully expanded (factor = 1.0)
- **Frame N**: Back to contracted (seamless loop)

## 🏗️ Architecture

### Core Components

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ Shape Generator │───▶│ JSON Storage    │───▶│ Point Renderer  │
│ - Heart curve   │    │ - Persistence   │    │ - Dark purple   │
│ - Star curve    │    │ - Load/Save     │    │   background    │
│ - Color gradient│    │ - Error handling│    │ - Pixel plotting│
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ Point Transform │    │ Centroid Calc   │    │ GIF Creation    │
│ - Scale toward  │    │ - Geometric     │    │ - ImageMagick   │
│   center        │    │   center        │    │ - Frame delay   │
│ - Preserve      │    │ - Animation     │    │ - Loop settings │
│   topology      │    │   anchor        │    │                 │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Data Structures

```cpp
// Heart particle
struct HeartPoint {
  double x, y;              // Coordinates
  unsigned char r, g, b;    // RGB color [0-255]
};

// Star particle (identical structure)
struct StarPoint {
  double x, y;              // Coordinates  
  unsigned char r, g, b;    // RGB color [0-255]
};
```

### JSON Format

```json
{
  "points": [
    {"x": 250.5, "y": 300.2, "r": 255, "g": 100, "b": 150},
    {"x": 251.3, "y": 301.8, "r": 240, "g": 90, "b": 140},
    ...
  ]
}
```

## 🧪 Testing

### Property-Based Testing

The system includes comprehensive property-based tests:

```bash
# Compile and run animation tests
cd build
g++ -std=c++20 -I../include ../test_animation_cosine.cpp -o test_animation
./test_animation
```

**Tested Properties:**
- **Cosine Wave Correctness**: Contraction factors follow mathematical formula
- **Symmetry**: Animation cycles are symmetric
- **Continuity**: Smooth transitions between frames
- **Extrema**: Correct minimum/maximum contraction points
- **Periodicity**: Seamless looping

### Sample Test Output

```
=== Property-Based Test: Animation Cosine Wave ===
Feature: star-animation, Property 8
Validates: Requirements 4.3

--- Test 1: Standard parameters (30 frames, 0.15 amplitude) ---
Testing cosine wave property with 30 frames and amplitude 0.15
Testing cosine wave symmetry property...
Testing cosine wave extrema property...
  Min contraction: 0.85 at frame 0
  Max contraction: 1 at frame 15
Testing cosine wave continuity property...
  Maximum change between consecutive frames: 0.0314159
PASS: Standard parameters test

✓ All property tests PASSED
```

## 📁 Project Structure

```
.
├── README.md                   # This file
├── main.cpp                    # Animation pipeline
├── include/                    # Header files
│   ├── generate_heart_points.h    # Heart shape generation
│   ├── generate_star_points.h     # Star shape generation
│   ├── render_points.h            # Point rendering
│   ├── transform_*_points.h       # Animation transforms
│   ├── read_*_json.h              # JSON loading
│   ├── write_*_json.h             # JSON saving
│   ├── calculate_centroid.h       # Geometric center
│   └── create_gif_from_frames.h   # GIF creation
├── src/                        # Implementation files
│   └── *.cpp                   # All function implementations
├── data/                       # Generated data
│   ├── heart.json              # Heart particle data
│   └── star.json               # Star particle data
├── build/                      # Build artifacts & output
│   ├── *_frame_*.ppm           # Animation frames
│   ├── *_static.ppm            # Static renderings
│   └── *.gif                   # Animated GIFs
└── test_animation_cosine.cpp   # Property-based tests
```

## 🎯 Key Functions

### Shape Generation
- `generate_heart_points()` - Creates heart-shaped particle distribution
- `generate_star_points()` - Creates five-pointed star distribution

### Animation System
- `calculate_centroid()` - Finds geometric center for transformations
- `transform_heart_points()` - Scales heart particles for animation
- `transform_star_points()` - Scales star particles for animation

### Rendering & I/O
- `render_points()` - Draws particles to RGB image buffer
- `read_*_json()` - Loads particle data from JSON files
- `write_*_json()` - Saves particle data to JSON files
- `create_gif_from_frames()` - Converts PPM sequence to animated GIF

## 🔧 Configuration

### Animation Parameters

```cpp
// Configurable in main.cpp
const int num_frames = 30;                    // Animation length
const double contraction_amplitude = 0.15;   // Pulse strength (15%)
const int image_width = 500;                  // Output dimensions
const int image_height = 500;
const int point_radius = 1;                   // Particle size
```

### Background Color

```cpp
// Dark purple background (RGB: 40, 20, 60)
const unsigned char bg_r = 40;
const unsigned char bg_g = 20; 
const unsigned char bg_b = 60;
```

## 🎨 Visual Examples

### Heart Animation Frames
```
Frame 0:  ♥ (most contracted)
Frame 7:  ♥♥ (expanding)
Frame 15: ♥♥♥ (fully expanded)
Frame 22: ♥♥ (contracting)
Frame 29: ♥ (back to contracted)
```

### Color Gradients

**Heart**: Pink/White → Bright Pink → Dark Red/Pink
**Star**: Yellow/White → Yellow/Orange → Orange/Red

## 🚀 Advanced Features

### Automatic JSON Management
- Generates particle data on first run
- Reuses existing data for consistency
- Graceful error handling for malformed files

### ImageMagick Integration
- Automatic GIF creation when ImageMagick is available
- Configurable frame delay (default: 3 centiseconds)
- Infinite loop settings for seamless playback

### Mathematical Precision
- Double-precision coordinates for smooth animation
- Floating-point tolerance in property tests
- Consistent frame-to-frame transformations

## 📊 Performance

- **Generation**: ~5000 particles per shape
- **Rendering**: Single-pass pixel plotting
- **Memory**: Pre-allocated vectors for efficiency
- **Animation**: 30 frames @ ~33 FPS equivalent

## 🔍 Troubleshooting

### Common Issues

**No GIF output**: Install ImageMagick
```bash
# macOS
brew install imagemagick

# Ubuntu
sudo apt-get install imagemagick
```

**Build errors**: Ensure C++20 support
```bash
# Check compiler version
g++ --version  # Should be ≥ 10.0
clang++ --version  # Should be ≥ 10.0
```

**Missing JSON files**: Files are auto-generated on first run
```bash
# Force regeneration
rm ../data/heart.json ../data/star.json
./raster
```

## 📈 Future Enhancements

- **Multiple Shapes**: Triangle, hexagon, custom curves
- **Physics Simulation**: Velocity, acceleration, collision
- **Interactive Controls**: Real-time parameter adjustment
- **Video Export**: MP4/WebM output formats
- **3D Animation**: Z-axis transformations
- **Color Animation**: Dynamic gradient shifts

---

*Built with C++20 • Tested with Property-Based Testing • Animated with Mathematics*