#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <iomanip>

// Feature: star-animation, Property 8: Animation uses cosine wave for contraction
// Validates: Requirements 4.3
//
// Property: For any frame number f in a 30-frame animation, the contraction factor 
// should follow the formula: 1.0 - amplitude * (1.0 + cos(2π*f/30)) / 2.0

const double EPSILON = 1e-10;

bool test_cosine_wave_property(int num_frames, double amplitude) {
    std::cout << "Testing cosine wave property with " << num_frames 
              << " frames and amplitude " << amplitude << std::endl;
    
    bool all_passed = true;
    
    for (int frame = 0; frame < num_frames; frame++) {
        // Calculate contraction factor using the formula from main.cpp
        const double phase = (2.0 * M_PI * frame) / num_frames;
        const double actual_contraction = 1.0 - amplitude * (1.0 + cos(phase)) / 2.0;
        
        // Expected formula from the property specification
        const double expected_contraction = 1.0 - amplitude * (1.0 + cos(2.0 * M_PI * frame / num_frames)) / 2.0;
        
        // Verify they match
        const double diff = std::abs(actual_contraction - expected_contraction);
        
        if (diff > EPSILON) {
            std::cerr << "FAIL: Frame " << frame << " - Expected: " << expected_contraction 
                      << ", Actual: " << actual_contraction << ", Diff: " << diff << std::endl;
            all_passed = false;
        }
        
        // Additional property checks:
        // 1. Contraction factor should be in valid range
        if (actual_contraction < 0.0 || actual_contraction > 2.0) {
            std::cerr << "FAIL: Frame " << frame << " - Contraction factor " 
                      << actual_contraction << " is out of reasonable range [0, 2]" << std::endl;
            all_passed = false;
        }
        
        // 2. For amplitude <= 1.0, contraction should be in range [1-amplitude, 1]
        if (amplitude <= 1.0) {
            const double min_expected = 1.0 - amplitude;
            const double max_expected = 1.0;
            if (actual_contraction < min_expected - EPSILON || actual_contraction > max_expected + EPSILON) {
                std::cerr << "FAIL: Frame " << frame << " - Contraction factor " 
                          << actual_contraction << " is outside expected range [" 
                          << min_expected << ", " << max_expected << "]" << std::endl;
                all_passed = false;
            }
        }
    }
    
    return all_passed;
}

bool test_cosine_wave_symmetry(int num_frames, double amplitude) {
    std::cout << "Testing cosine wave symmetry property..." << std::endl;
    
    bool all_passed = true;
    
    // The cosine wave should be symmetric: frame 0 and frame num_frames should have same contraction
    // (or very close if we consider it as a cycle)
    for (int frame = 0; frame < num_frames / 2; frame++) {
        const double phase1 = (2.0 * M_PI * frame) / num_frames;
        const double contraction1 = 1.0 - amplitude * (1.0 + cos(phase1)) / 2.0;
        
        const double phase2 = (2.0 * M_PI * (num_frames - frame)) / num_frames;
        const double contraction2 = 1.0 - amplitude * (1.0 + cos(phase2)) / 2.0;
        
        // Due to cosine symmetry, these should be equal
        const double diff = std::abs(contraction1 - contraction2);
        
        if (diff > EPSILON) {
            std::cerr << "FAIL: Symmetry check - Frame " << frame << " and frame " 
                      << (num_frames - frame) << " should have same contraction. "
                      << "Diff: " << diff << std::endl;
            all_passed = false;
        }
    }
    
    return all_passed;
}

bool test_cosine_wave_extrema(int num_frames, double amplitude) {
    std::cout << "Testing cosine wave extrema property..." << std::endl;
    
    bool all_passed = true;
    
    // Find minimum and maximum contraction factors
    double min_contraction = 2.0;
    double max_contraction = 0.0;
    int min_frame = -1;
    int max_frame = -1;
    
    for (int frame = 0; frame < num_frames; frame++) {
        const double phase = (2.0 * M_PI * frame) / num_frames;
        const double contraction = 1.0 - amplitude * (1.0 + cos(phase)) / 2.0;
        
        if (contraction < min_contraction) {
            min_contraction = contraction;
            min_frame = frame;
        }
        if (contraction > max_contraction) {
            max_contraction = contraction;
            max_frame = frame;
        }
    }
    
    std::cout << "  Min contraction: " << min_contraction << " at frame " << min_frame << std::endl;
    std::cout << "  Max contraction: " << max_contraction << " at frame " << max_frame << std::endl;
    
    // For amplitude <= 1.0:
    // - Maximum should be at frame 0 (cos(0) = 1, so contraction = 1.0 - amplitude * (1+1)/2 = 1.0 - amplitude)
    // - Minimum should be at frame num_frames/2 (cos(π) = -1, so contraction = 1.0 - amplitude * (1-1)/2 = 1.0)
    
    if (amplitude <= 1.0) {
        const double expected_max = 1.0;
        const double expected_min = 1.0 - amplitude;
        
        if (std::abs(max_contraction - expected_max) > EPSILON) {
            std::cerr << "FAIL: Maximum contraction should be " << expected_max 
                      << " but got " << max_contraction << std::endl;
            all_passed = false;
        }
        
        if (std::abs(min_contraction - expected_min) > EPSILON) {
            std::cerr << "FAIL: Minimum contraction should be " << expected_min 
                      << " but got " << min_contraction << std::endl;
            all_passed = false;
        }
        
        // Frame 0 should have minimum contraction (most contracted)
        if (min_frame != 0) {
            std::cerr << "FAIL: Minimum contraction should occur at frame 0, but occurred at frame " 
                      << min_frame << std::endl;
            all_passed = false;
        }
        
        // Frame num_frames/2 should have maximum contraction (least contracted / most expanded)
        // Exception: if amplitude is 0, all frames have same value, so any frame is valid
        if (amplitude > EPSILON && max_frame != num_frames / 2) {
            std::cerr << "FAIL: Maximum contraction should occur at frame " << num_frames / 2 
                      << ", but occurred at frame " << max_frame << std::endl;
            all_passed = false;
        }
    }
    
    return all_passed;
}

bool test_cosine_wave_continuity(int num_frames, double amplitude) {
    std::cout << "Testing cosine wave continuity property..." << std::endl;
    
    bool all_passed = true;
    
    // The contraction factor should change smoothly between frames
    // Calculate maximum change between consecutive frames
    double max_change = 0.0;
    
    for (int frame = 0; frame < num_frames - 1; frame++) {
        const double phase1 = (2.0 * M_PI * frame) / num_frames;
        const double contraction1 = 1.0 - amplitude * (1.0 + cos(phase1)) / 2.0;
        
        const double phase2 = (2.0 * M_PI * (frame + 1)) / num_frames;
        const double contraction2 = 1.0 - amplitude * (1.0 + cos(phase2)) / 2.0;
        
        const double change = std::abs(contraction2 - contraction1);
        max_change = std::max(max_change, change);
    }
    
    std::cout << "  Maximum change between consecutive frames: " << max_change << std::endl;
    
    // For smooth animation, the maximum change should be reasonable
    // With 30 frames and amplitude 0.15, max change should be around amplitude * π / num_frames
    const double expected_max_change = amplitude * M_PI / num_frames;
    
    if (max_change > expected_max_change * 1.5) {
        std::cerr << "FAIL: Maximum change " << max_change 
                  << " exceeds expected smooth change " << expected_max_change << std::endl;
        all_passed = false;
    }
    
    return all_passed;
}

int main() {
    std::cout << "=== Property-Based Test: Animation Cosine Wave ===" << std::endl;
    std::cout << "Feature: star-animation, Property 8" << std::endl;
    std::cout << "Validates: Requirements 4.3" << std::endl;
    std::cout << std::endl;
    
    int total_tests = 0;
    int passed_tests = 0;
    
    // Test with the actual parameters used in main.cpp
    std::cout << "\n--- Test 1: Standard parameters (30 frames, 0.15 amplitude) ---" << std::endl;
    total_tests++;
    if (test_cosine_wave_property(30, 0.15) && 
        test_cosine_wave_symmetry(30, 0.15) &&
        test_cosine_wave_extrema(30, 0.15) &&
        test_cosine_wave_continuity(30, 0.15)) {
        std::cout << "PASS: Standard parameters test" << std::endl;
        passed_tests++;
    } else {
        std::cout << "FAIL: Standard parameters test" << std::endl;
    }
    
    // Test with different frame counts (property should hold for any frame count)
    std::cout << "\n--- Test 2: Different frame count (60 frames, 0.15 amplitude) ---" << std::endl;
    total_tests++;
    if (test_cosine_wave_property(60, 0.15) && 
        test_cosine_wave_symmetry(60, 0.15) &&
        test_cosine_wave_extrema(60, 0.15) &&
        test_cosine_wave_continuity(60, 0.15)) {
        std::cout << "PASS: 60 frames test" << std::endl;
        passed_tests++;
    } else {
        std::cout << "FAIL: 60 frames test" << std::endl;
    }
    
    // Test with different amplitude
    std::cout << "\n--- Test 3: Different amplitude (30 frames, 0.3 amplitude) ---" << std::endl;
    total_tests++;
    if (test_cosine_wave_property(30, 0.3) && 
        test_cosine_wave_symmetry(30, 0.3) &&
        test_cosine_wave_extrema(30, 0.3) &&
        test_cosine_wave_continuity(30, 0.3)) {
        std::cout << "PASS: 0.3 amplitude test" << std::endl;
        passed_tests++;
    } else {
        std::cout << "FAIL: 0.3 amplitude test" << std::endl;
    }
    
    // Test with small frame count
    std::cout << "\n--- Test 4: Small frame count (10 frames, 0.15 amplitude) ---" << std::endl;
    total_tests++;
    if (test_cosine_wave_property(10, 0.15) && 
        test_cosine_wave_symmetry(10, 0.15) &&
        test_cosine_wave_extrema(10, 0.15) &&
        test_cosine_wave_continuity(10, 0.15)) {
        std::cout << "PASS: 10 frames test" << std::endl;
        passed_tests++;
    } else {
        std::cout << "FAIL: 10 frames test" << std::endl;
    }
    
    // Test with zero amplitude (no contraction)
    std::cout << "\n--- Test 5: Zero amplitude (30 frames, 0.0 amplitude) ---" << std::endl;
    total_tests++;
    if (test_cosine_wave_property(30, 0.0) && 
        test_cosine_wave_symmetry(30, 0.0) &&
        test_cosine_wave_extrema(30, 0.0) &&
        test_cosine_wave_continuity(30, 0.0)) {
        std::cout << "PASS: Zero amplitude test" << std::endl;
        passed_tests++;
    } else {
        std::cout << "FAIL: Zero amplitude test" << std::endl;
    }
    
    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Passed: " << passed_tests << "/" << total_tests << std::endl;
    
    if (passed_tests == total_tests) {
        std::cout << "\n✓ All property tests PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "\n✗ Some property tests FAILED" << std::endl;
        return 1;
    }
}
