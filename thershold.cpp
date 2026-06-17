#include "threshold.h"
#include <cstring> // ضروري عشان نستخدم memset

void applyDoubleThreshold(const uint8_t* nmsImage,
                          uint8_t* thresholdResult,
                          int width,
                          int height,
                          uint8_t lowThreshold,
                          uint8_t highThreshold) {
    
    int size = width * height;
    
    // تصفير المصفوفة بالكامل قبل البدأ
    std::memset(thresholdResult, 0, size * sizeof(uint8_t));

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