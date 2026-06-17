#include "direction.h"
#include <cstdlib>

void computeDirection(const std::vector<int16_t>& Gx,
                      const std::vector<int16_t>& Gy,
                      std::vector<uint8_t>& direction,
                      int width, int height) {
    
    int size = width * height;
    direction.assign(size, 0);

    for (int i = 0; i < size; ++i) {
        int32_t ax = std::abs(Gx[i]);
        int32_t ay = std::abs(Gy[i]);

        // Cross-multiplication optimization avoiding floats
        int32_t ay5 = ay * 5;
        
        if (ay5 < ax * 2) {
            direction[i] = 0;       // Horizontal
        } else if (ay5 > ax * 12) {
            direction[i] = 90;      // Vertical
        } else {
            // Check signs to distinguish between 45 and 135
            int16_t gx = Gx[i];
            int16_t gy = Gy[i];
            if ((gx > 0 && gy > 0) || (gx < 0 && gy < 0)) {
                direction[i] = 135;
            } else {
                direction[i] = 45;
            }
        }
    }
}