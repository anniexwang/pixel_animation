# Implementation Plan

- [x] 1. Create StarPoint structure and star point generation
  - Create `include/generate_star_points.h` with StarPoint struct and function declaration
  - Implement `src/generate_star_points.cpp` with five-pointed star parametric generation
  - Use random distribution for particle scatter and jitter
  - Implement distance-based color gradient (yellow/white edges to orange/red center)
  - Ensure all generated points fall within image bounds
  - _Requirements: 1.1, 1.2, 1.3, 1.4, 1.5_

- [ ]* 1.1 Write property test for star point generation
  - **Property 1: Star particle generation produces valid coordinates**
  - **Validates: Requirements 1.5**

- [ ]* 1.2 Write property test for color gradient
  - **Property 2: Star particles follow distance-based color gradient**
  - **Validates: Requirements 1.3**

- [x] 2. Implement JSON serialization for star points
  - Create `include/write_star_json.h` and `src/write_star_json.cpp`
  - Create `include/read_star_json.h` and `src/read_star_json.cpp`
  - Use nlohmann json library to serialize/deserialize StarPoint vectors
  - Return bool to indicate success/failure
  - Handle file I/O errors gracefully
  - _Requirements: 2.1, 2.2_

- [ ]* 2.1 Write property test for JSON round-trip
  - **Property 3: JSON serialization round-trip preserves data**
  - **Validates: Requirements 2.1, 2.2**

- [x] 3. Implement star point transformation
  - Create `include/transform_star_points.h` and `src/transform_star_points.cpp`
  - Implement scaling transformation relative to center point
  - Formula: new_pos = center + scale_factor * (old_pos - center)
  - Preserve color values during transformation
  - _Requirements: 4.2_

- [ ]* 3.1 Write property test for transformation
  - **Property 7: Transformation scales particles relative to center**
  - **Validates: Requirements 4.2**

- [ ]* 3.2 Write property test for centroid calculation
  - **Property 6: Centroid calculation finds geometric center**
  - **Validates: Requirements 4.1**

- [x] 4. Implement GIF creation from frames
  - Create `include/create_gif_from_frames.h` and `src/create_gif_from_frames.cpp`
  - Use system() call to invoke ImageMagick convert command
  - Accept vector of frame file paths and output GIF path
  - Set frame delay parameter (default 3 centiseconds)
  - Return bool to indicate success/failure
  - _Requirements: 3.4, 3.5_

- [x] 5. Integrate star animation into main.cpp
  - Add includes for all star-related headers
  - Generate star.json if it doesn't exist (500x500 image, 5000 particles)
  - Load star points from star.json
  - Render static star image to star_static.ppm
  - Calculate star centroid using existing calculate_centroid function
  - Generate 30 animation frames with cosine wave contraction
  - Save frames as star_frame_000.ppm through star_frame_029.ppm
  - Create animated GIF from frames: star_animation.gif
  - Add console output for progress tracking
  - _Requirements: 2.3, 2.4, 4.1, 4.3, 4.4, 4.5, 5.2, 5.3, 5.4_

- [ ]* 5.1 Write property test for rendering
  - **Property 4: Rendering places particles at correct positions**
  - **Validates: Requirements 3.1**

- [ ]* 5.2 Write property test for particle radius
  - **Property 5: Particle radius affects rendered output**
  - **Validates: Requirements 3.2**

- [x] 5.3 Write property test for animation cosine wave
  - **Property 8: Animation uses cosine wave for contraction**
  - **Validates: Requirements 4.3**

- [x] 6. Update CMakeLists.txt
  - Add new source files to the build:
    - src/generate_star_points.cpp
    - src/write_star_json.cpp
    - src/read_star_json.cpp
    - src/transform_star_points.cpp
    - src/create_gif_from_frames.cpp
  - _Requirements: 5.1_

- [x] 7. Checkpoint - Ensure all tests pass
  - Ensure all tests pass, ask the user if questions arise.
