# Implementation Plan

- [x] 1. Create core data structures and heart generation
- [x] 1.1 Define HeartPoint struct and create header files
  - Define HeartPoint struct with x, y, r, g, b fields
  - Create include/generate_heart_points.h
  - Create include/write_heart_json.h
  - _Requirements: 6.1, 6.2_

- [ ] 1.2 Implement parametric heart curve generation with colors
  - Write function to generate ~2000-3000 points along heart curve
  - Use parametric equations: x(t) = 16*sin³(t), y(t) = 13*cos(t) - 5*cos(2t) - 2*cos(3t) - cos(4t)
  - Add random offsets for particle effect
  - Assign color gradient (bright pink/white center to dark red/pink edges)
  - _Requirements: 1.4, 1.5, 1.6_

- [ ] 1.3 Implement JSON writer and generate heart.json
  - Use json.hpp to write points to data/heart.json
  - Format: array of objects with x, y, r, g, b fields
  - _Requirements: 1.7, 6.4_

- [x] 2. Implement JSON reader and basic rendering
- [x] 2.1 Implement JSON reader with file existence check
  - Check if data/heart.json exists
  - Parse JSON and extract points
  - Validate required fields and handle errors
  - _Requirements: 1.1, 1.2, 1.3, 1.8, 1.9_

- [x] 2.2 Implement basic point rendering
  - Create include/render_points.h
  - Initialize image with dark purple background (40, 20, 60)
  - Render each point as single pixel at its position
  - Skip out-of-bounds points
  - _Requirements: 4.1, 4.2, 5.1, 5.2_

- [x] 3. Create first static heart image
- [x] 3.1 Integrate heart generation and rendering in main.cpp
  - Add heart animation code to main.cpp
  - Generate heart.json if it doesn't exist
  - Load points from heart.json
  - Render single frame as heart_static.ppm
  - Print status messages
  - _Requirements: 1.1, 1.2, 1.3, 6.3_

- [x] 3.2 Build and test static heart output
  - Compile the project
  - Run and verify heart_static.ppm is created
  - Visually inspect the heart shape and colors
  - _Requirements: 6.5_

- [x] 4. Implement animation transformation
- [x] 4.1 Implement centroid calculation
  - Create include/calculate_centroid.h
  - Calculate average x and y of all points
  - _Requirements: 7.1, 7.2_

- [x] 4.2 Implement contraction transformation
  - Create include/transform_heart_points.h
  - Calculate contraction factor using cosine: factor = 1.0 - amplitude * (1 + cos(phase)) / 2
  - Transform points toward center: new_pos = center + (pos - center) * factor
  - _Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 7.3_

- [x] 5. Generate animation frames
- [x] 5.1 Implement frame generation loop
  - Generate 30 frames by default
  - For each frame: calculate contraction, transform points, render, write PPM
  - Name files: heart_frame_000.ppm, heart_frame_001.ppm, etc.
  - Print progress for each frame
  - _Requirements: 3.1, 3.2, 3.3, 3.4, 3.5_

- [x] 5.2 Build and test animation output
  - Compile and run the program
  - Verify 30 frames are generated
  - Visually inspect frames to confirm pulsing effect
  - Check that animation loops smoothly
  - _Requirements: All_

- [x] 6. Checkpoint - Verify animation works
  - Ensure all frames are generated correctly
  - Confirm heart shape is recognizable
  - Verify smooth pulsing motion
  - Ask user if questions arise

- [ ]* 7. Add property-based tests (optional)
- [ ]* 7.1 Write property test for JSON round-trip
  - **Property 1: JSON round-trip preservation**
  - **Validates: Requirements 1.7, 1.8**

- [ ]* 7.2 Write property test for color gradient
  - **Property 2: Color gradient correctness**
  - **Validates: Requirements 1.5**

- [ ]* 7.3 Write property test for contraction toward center
  - **Property 4: Contraction moves points toward center**
  - **Validates: Requirements 2.2**

- [ ]* 7.4 Write property test for centroid calculation
  - **Property 15: Centroid calculation correctness**
  - **Validates: Requirements 7.1, 7.2**

- [ ]* 8. Add remaining property tests (optional)
- [ ]* 8.1 Write property tests for transformation properties
  - **Property 5: Contraction factor periodicity**
  - **Property 6: Topology preservation**
  - **Property 7: Identity transformation**
  - **Property 16: Centroid as contraction center**
  - **Validates: Requirements 2.3, 2.4, 2.5, 3.4, 7.3**

- [ ]* 8.2 Write property tests for rendering and frames
  - **Property 8: Frame count matches request**
  - **Property 9: Sequential frame numbering**
  - **Property 10: Uniform contraction distribution**
  - **Property 11: Out-of-bounds points skipped**
  - **Property 12: Point radius affects size**
  - **Property 13: Background color initialization**
  - **Property 14: RGB value validation**
  - **Validates: Requirements 3.1, 3.3, 3.5, 4.2, 4.5, 5.1, 5.4**

- [ ]* 9. Final checkpoint - Ensure all tests pass
  - Run all property-based tests
  - Ensure all tests pass, ask user if questions arise
