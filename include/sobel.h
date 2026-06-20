#ifndef SOBEL_H
#define SOBEL_H

#include <cstdint>

void applySobel(const uint8_t* inputImage,
                int16_t* Gx,
                int16_t* Gy,
                int width, int height);

#endif // SOBEL_H