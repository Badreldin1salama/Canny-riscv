#include "magnitude.h"
#include <cstdlib>

void computeMagnitude(const std::vector<int16_t>& Gx,
                      const std::vector<int16_t>& Gy,
                      std::vector<uint8_t>& magnitude,
                      int width, int height) {
    
    int size = width * height;
    magnitude.assign(size, 0);
    
    std::vector<int32_t> tempMag(size, 0);
    int32_t maxMag = 0;

    // Pass 1: L1 norm (|Gx| + |Gy|) and find maximum value
    for (int i = 0; i < size; ++i) {
        int32_t mag = std::abs(Gx[i]) + std::abs(Gy[i]);
        tempMag[i] = mag;
        if (mag > maxMag) {
            maxMag = mag;
        }
    }

    // Pass 2: Normalize to [0, 255]
    if (maxMag == 0) maxMag = 1; // Prevent division by zero
    for (int i = 0; i < size; ++i) {
        magnitude[i] = (uint8_t)((tempMag[i] * 255) / maxMag);
    }
}