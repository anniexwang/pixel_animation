# Project Structure

## Directory Layout

```
.
├── CMakeLists.txt          # Build configuration
├── main.cpp                # Entry point, orchestrates all operations
├── include/                # Header files (DO NOT MODIFY)
│   ├── *.h                 # Function declarations with documentation
│   └── stb_image.h         # Third-party image loading library
├── src/                    # Implementation files (STUDENT WORK HERE)
│   └── *.cpp               # Function implementations
├── data/                   # Sample input images
│   └── validation/         # Reference PPM files for validation
├── build/                  # Build artifacts (generated)
├── json/                   # JSON library
└── images/                 # Documentation images for README
```

## Code Organization

### Headers (`include/`)
- One header file per function
- Contains function signature and detailed documentation
- Uses include guards (`#ifndef`, `#define`, `#endif`)
- **Never modify these files** - they define the API contract

### Source Files (`src/`)
- One `.cpp` file per header
- Contains implementation of the function declared in corresponding header
- Students implement code between comment markers:
  ```cpp
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  ```

### Main Program (`main.cpp`)
- Includes all headers from `include/`
- Links to compiled functions from `src/`
- Orchestrates the image processing pipeline
- Accepts command-line arguments for input files or uses defaults

## Image Data Representation

Images are stored as **flattened 1D vectors** (`std::vector<unsigned char>`):
- **RGB image**: `width * height * 3` elements (R, G, B interleaved)
- **RGBA image**: `width * height * 4` elements (R, G, B, A interleaved)
- **Grayscale**: `width * height` elements

### Memory Layout
Row-major order: pixels stored left-to-right, top-to-bottom.

Example: Access green value at row 36, column 89 in 767×772 RGB image:
```cpp
data[1 + 3*(88 + 767*35)]  // Remember: 0-indexed
```

## Naming Conventions

- **Functions**: snake_case (e.g., `rgb_to_gray`, `hue_shift`)
- **Variables**: snake_case (e.g., `width`, `height`, `num_channels`)
- **Constants**: Not explicitly defined, but use descriptive names
- **Files**: Match function names (e.g., `rgb_to_gray.h`, `rgb_to_gray.cpp`)

## Key Functions by Category

### Color Conversion
- `rgba_to_rgb.cpp` - Extract RGB from RGBA
- `rgb_to_gray.cpp` - Convert to grayscale (weighted average)
- `rgb_to_hsv.cpp` / `hsv_to_rgb.cpp` - HSV color space conversion

### Transformations
- `reflect.cpp` - Horizontal mirror
- `rotate.cpp` - 90° counter-clockwise rotation

### Bayer Pattern
- `simulate_bayer_mosaic.cpp` - Create GBRG mosaic from RGB
- `demosaic.cpp` - Reconstruct RGB from mosaic via interpolation

### Color Adjustments
- `hue_shift.cpp` - Shift hue by degrees
- `desaturate.cpp` - Reduce saturation by factor

### Compositing
- `over.cpp` - Porter-Duff Over operator for alpha blending

### I/O
- `write_ppm.cpp` - Write PPM file format
- `read_rgba_from_png.h` - Read PNG files (header-only wrapper)
