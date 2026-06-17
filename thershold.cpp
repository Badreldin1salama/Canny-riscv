#include "threshold.h"

void applyDoubleThreshold(const std::vector<uint8_t>& nmsImage,
                          std::vector<uint8_t>& thresholdResult,
                          int width,
                          int height,
                          uint8_t lowThreshold,
                          uint8_t highThreshold) {
    
    int size = width * height;
    thresholdResult.assign(size, 0);

    // Single pass over the image, perfect for SIMD/Vectorization later
    for (int i = 0; i < size; ++i) {
        uint8_t val = nmsImage[i];
        
        if (val >= highThreshold) {
            thresholdResult[i] = 255; // Strong edge (White)
        } else if (val >= lowThreshold) {
            thresholdResult[i] = 128; // Weak edge (Gray)
        } else {
            thresholdResult[i] = 0;   // Noise / Non-edge (Black)
        }
    }
}