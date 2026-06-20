#ifndef DIRECTION_H
#define DIRECTION_H

#include <cstdint>

void computeDirection(const int16_t* Gx,
                      const int16_t* Gy,
                      uint8_t* direction,
                      int width, int height);

#endif // DIRECTION_H