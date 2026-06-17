#include "nms.h"

void applyNMS(const std::vector<uint8_t>& magnitude,
              const std::vector<uint8_t>& direction,
              std::vector<uint8_t>& nmsResult,
              int width,
              int height) {
    
    // Allocate memory and initialize with 0
    nmsResult.assign(width * height, 0);

    // Skip the outer border to avoid out-of-bounds errors (Boundary Handling)
    for (int row = 1; row < height - 1; ++row) {
        for (int col = 1; col < width - 1; ++col) {
            
            int flatIndex = row * width + col;
            uint8_t mag = magnitude[flatIndex];
            uint8_t dir = direction[flatIndex];

            // If it's not an edge at all, skip it (Optimization)
            if (mag == 0) continue;

            uint8_t neighbor1 = 0;
            uint8_t neighbor2 = 0;

            // Determine neighbors based on gradient direction
            if (dir == 0) {
                // Horizontal gradient -> Vertical edge
                // Compare with pixels above and below
                neighbor1 = magnitude[(row - 1) * width + col];
                neighbor2 = magnitude[(row + 1) * width + col];
            } 
            else if (dir == 90) {
                // Vertical gradient -> Horizontal edge
                // Compare with pixels left and right
                neighbor1 = magnitude[row * width + (col - 1)];
                neighbor2 = magnitude[row * width + (col + 1)];
            } 
            else if (dir == 45) {
                // Diagonal 45 -> Compare top-right and bottom-left
                neighbor1 = magnitude[(row - 1) * width + (col + 1)];
                neighbor2 = magnitude[(row + 1) * width + (col - 1)];
            } 
            else if (dir == 135) {
                // Diagonal 135 -> Compare top-left and bottom-right
                neighbor1 = magnitude[(row - 1) * width + (col - 1)];
                neighbor2 = magnitude[(row + 1) * width + (col + 1)];
            }

            // Suppress if the current pixel is not the local maximum
            if (mag >= neighbor1 && mag >= neighbor2) {
                nmsResult[flatIndex] = mag;
            } else {
                nmsResult[flatIndex] = 0;
            }
        }
    }
}
