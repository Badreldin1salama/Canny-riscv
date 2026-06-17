#include "magnitude.h"
#include <cstdlib>
#include <cmath> // Added for std::abs

void computeMagnitude(const int16_t* Gx,
                      const int16_t* Gy,
                      uint8_t* magnitude,
                      int width, int height) {
    
    int size = width * height;
    int32_t maxMag = 0;

    // Pass 1: L1 norm (|Gx| + |Gy|) and find maximum value
    for (int i = 0; i < size; ++i) {
        int32_t mag = std::abs(Gx[i]) + std::abs(Gy[i]);
        if (mag > maxMag) {
            maxMag = mag;
        }
    }

    // Pass 2: Recalculate and Normalize to [0, 255]
    if (maxMag == 0) maxMag = 1; // Prevent division by zero
    
    for (int i = 0; i < size; ++i) {
        // إعادة الحساب هنا أسرع بكتير من تخزين مصفوفة كاملة في الذاكرة
        int32_t mag = std::abs(Gx[i]) + std::abs(Gy[i]);
        magnitude[i] = static_cast<uint8_t>((mag * 255) / maxMag);
    }
}