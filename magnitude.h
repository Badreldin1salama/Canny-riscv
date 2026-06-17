#ifndef MAGNITUDE_H
#define MAGNITUDE_H

#include <vector>
#include <cstdint>

void computeMagnitude(const std::vector<int16_t>& Gx,
                      const std::vector<int16_t>& Gy,
                      std::vector<uint8_t>& magnitude,
                      int width, int height);

#endif // MAGNITUDE_H