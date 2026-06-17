#ifndef MAGNITUDE_H
#define MAGNITUDE_H

#include <cstdint>

void computeMagnitude(const int16_t* Gx,
                      const int16_t* Gy,
                      uint8_t* magnitude,
                      int width, int height);

#endif // MAGNITUDE_H