#ifndef NMS_H
#define NMS_H

#include <cstdint>

void applyNMS(const uint8_t* magnitude,
              const uint8_t* direction,
              uint8_t* nmsResult,
              int width,
              int height);

#endif // NMS_H