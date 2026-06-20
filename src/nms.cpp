#include "nms.h"
#include <cstring> // Added for std::memset

void applyNMS(const uint8_t* magnitude,
              const uint8_t* direction,
              uint8_t* nmsResult,
              int width,
              int height) {
    
    int size = width * height;

    // تصفير المصفوفة بالكامل لضمان إن بيكسلات الحدود (Borders) تفضل بصفر
    std::memset(nmsResult, 0, size * sizeof(uint8_t));

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
                // Horizontal gradient -> compare with LEFT and RIGHT neighbors
                neighbor1 = magnitude[row * width + (col - 1)];
                neighbor2 = magnitude[row * width + (col + 1)];
            } 
            else if (dir == 90) {
                // Vertical gradient -> compare with TOP and BOTTOM neighbors
                neighbor1 = magnitude[(row - 1) * width + col];
                neighbor2 = magnitude[(row + 1) * width + col];
            } 
            else if (dir == 45) {
                // Diagonal 45 -> compare with Top-Right and Bottom-Left
                neighbor1 = magnitude[(row - 1) * width + (col + 1)];
                neighbor2 = magnitude[(row + 1) * width + (col - 1)];
            } 
            else if (dir == 135) {
                // Diagonal 135 -> compare with Top-Left and Bottom-Right
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