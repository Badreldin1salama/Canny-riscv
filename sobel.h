#ifndef SOBEL_H
#define SOBEL_H

#include <vector>
#include <cstdint>

void applySobel(const std::vector<uint8_t>& inputImage,
                std::vector<int16_t>& Gx,
                std::vector<int16_t>& Gy,
                int width, int height);

#endif // SOBEL_H