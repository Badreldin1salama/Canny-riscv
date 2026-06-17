#ifndef DIRECTION_H
#define DIRECTION_H

#include <vector>
#include <cstdint>

void computeDirection(const std::vector<int16_t>& Gx,
                      const std::vector<int16_t>& Gy,
                      std::vector<uint8_t>& direction,
                      int width, int height);

#endif // DIRECTION_H