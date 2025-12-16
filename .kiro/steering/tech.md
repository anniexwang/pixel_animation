# Technology Stack

## Build System

- **CMake** (minimum version 3.20)
- **C++ Standard**: C++20 (set via `CMAKE_CXX_STANDARD`)
- **Compiler Support**: 
  - macOS: Xcode/clang
  - Linux: gcc/g++
  - Windows: Microsoft Visual Studio 2022+ (64-bit only)

## Libraries & Dependencies

- **STL**: Standard Template Library (`<algorithm>`, `<cmath>`, `<vector>`)
- **stb_image.h**: Single-header image loading library (in `include/`)
- **json.hpp**: JSON library from nlohmann (in `json/`)
- No external dependencies required beyond standard C++ libraries

## File Formats

- **Input**: PNG files (RGBA) via stb_image
- **Output**: PPM files (text-based, uncompressed)
  - Supports RGB (3-channel) and grayscale (1-channel)

## Common Commands

### Initial Setup
```bash
git clone --recursive <repo-url>
# If already cloned without --recursive:
git submodule update --init --recursive
```

### Build
```bash
mkdir build
cd build
cmake ..
make              # macOS/Linux
# or open raster.sln in Visual Studio (Windows)
```

### Run
```bash
./raster                    # Uses default images from ../data/
./raster ../data/dog.png    # Process specific image
```

### Validation
Compare output PPM files in `build/` directory against reference files in `data/validation/`. RGB values should match exactly.

## Compiler Warnings

The project enables strict warnings:
- MSVC: `/W4 /permissive-`
- GCC/Clang: `-Wall -Wextra -Wpedantic`
