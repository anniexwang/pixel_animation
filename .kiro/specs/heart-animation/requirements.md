# Requirements Document

## Introduction

This feature extends the existing raster image processing system to support particle-based heart animation. The system will read point coordinates with colors from a JSON file and generate a sequence of PPM image frames showing a pulsing heart effect, where the heart contracts and expands in a rhythmic pattern.

## Glossary

- **Point**: A 2D coordinate (x, y) with an associated RGB color value
- **Frame**: A single image in the animation sequence
- **Animation Sequence**: A series of frames that create the illusion of motion when displayed sequentially
- **Contraction Factor**: A scalar value that determines how much the heart shrinks toward its center during the pulse
- **Particle System**: A collection of colored points that form the visual representation of the heart
- **JSON Input**: A structured data file containing point coordinates and color information
- **PPM Output**: Portable Pixmap format files for each animation frame

## Requirements

### Requirement 1

**User Story:** As a user, I want the system to auto-generate a heart.json file with point data that recreates the heart shape from the reference screenshot, so that I have a ready-to-use heart animation.

#### Acceptance Criteria

1. WHEN the program starts THEN the system SHALL check if heart.json exists in the data directory
2. IF heart.json does not exist THEN the system SHALL generate it once with heart-shaped point data
3. IF heart.json already exists THEN the system SHALL load and use the existing file without regenerating
4. WHEN generating the heart shape THEN the system SHALL create points that form a heart silhouette matching the reference screenshot
5. WHEN assigning colors to points THEN the system SHALL use a gradient from bright pink/white in the center to darker red/pink at the edges
6. WHEN distributing points THEN the system SHALL create a particle-like appearance with varying densities (denser at edges, sparser in center)
7. WHERE the JSON format is defined THEN the system SHALL use an array of objects with x, y, r, g, b fields
8. WHEN parsing the JSON file THEN the system SHALL extract point coordinates and RGB color values
9. WHEN the JSON file contains invalid data THEN the system SHALL report a clear error message and terminate gracefully

### Requirement 2

**User Story:** As a user, I want the heart to pulse with a contraction and expansion effect, so that the animation appears lifelike and dynamic.

#### Acceptance Criteria

1. WHEN generating animation frames THEN the system SHALL apply a time-varying contraction factor to point positions
2. WHEN contracting the heart THEN the system SHALL scale point positions toward the heart's center point
3. WHEN the contraction factor varies THEN the system SHALL use a smooth periodic function (e.g., sine or cosine wave)
4. WHEN calculating new point positions THEN the system SHALL preserve the relative spatial relationships between points
5. WHEN the heart is fully expanded THEN the system SHALL display points at their original positions

### Requirement 3

**User Story:** As a user, I want to generate multiple frames of the animation, so that I can create a smooth pulsing effect.

#### Acceptance Criteria

1. WHEN generating frames THEN the system SHALL create a configurable number of frames (default: 30 frames)
2. WHEN creating each frame THEN the system SHALL calculate the appropriate contraction factor for that time step
3. WHEN writing frames THEN the system SHALL output each frame as a separate PPM file with sequential numbering
4. WHEN the animation completes one cycle THEN the system SHALL ensure the last frame transitions smoothly back to the first frame
5. WHERE frame rate is specified THEN the system SHALL distribute contraction values evenly across the animation cycle

### Requirement 4

**User Story:** As a user, I want each point to be rendered as a visible pixel or small circle, so that the particle-based heart is clearly visible.

#### Acceptance Criteria

1. WHEN rendering a point THEN the system SHALL draw the point at its calculated position with its specified color
2. WHEN a point position falls outside image bounds THEN the system SHALL skip rendering that point
3. WHEN rendering points THEN the system SHALL support rendering each point as a single pixel or as a small filled circle
4. WHEN multiple points overlap THEN the system SHALL blend colors using alpha compositing or overwrite with the latest point
5. WHERE point size is configurable THEN the system SHALL allow specification of point radius (default: 1 pixel)

### Requirement 5

**User Story:** As a user, I want the background to be a solid color, so that the heart stands out clearly against the canvas.

#### Acceptance Criteria

1. WHEN creating each frame THEN the system SHALL initialize the image with a configurable background color
2. WHEN no background color is specified THEN the system SHALL use a default dark purple background (RGB: 40, 20, 60)
3. WHEN rendering points THEN the system SHALL draw points on top of the background
4. WHERE background color is provided THEN the system SHALL accept RGB values in the range [0, 255]

### Requirement 6

**User Story:** As a developer, I want the heart animation code to integrate with the existing codebase, so that it follows established patterns and conventions.

#### Acceptance Criteria

1. WHEN implementing new functions THEN the system SHALL follow the existing header/source file pattern
2. WHEN creating new source files THEN the system SHALL place headers in `include/` and implementations in `src/`
3. WHEN using existing functionality THEN the system SHALL reuse `write_ppm` for frame output
4. WHERE JSON parsing is needed THEN the system SHALL use the existing `json.hpp` library
5. WHEN building the project THEN the system SHALL integrate with the existing CMake build system

### Requirement 7

**User Story:** As a user, I want to calculate the heart's center point automatically, so that the contraction effect is centered correctly.

#### Acceptance Criteria

1. WHEN loading point data THEN the system SHALL calculate the centroid of all points
2. WHEN calculating the centroid THEN the system SHALL compute the average x and y coordinates of all points
3. WHEN contracting points THEN the system SHALL use the centroid as the center of contraction
4. WHERE center point is manually specified THEN the system SHALL allow override of the automatic centroid calculation
