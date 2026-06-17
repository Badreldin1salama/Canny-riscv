#ifndef NMS_H
#define NMS_H

#include <vector>
#include <cstdint>

void applyNMS(const std::vector<uint8_t>& magnitude,
              const std::vector<uint8_t>& direction,
              std::vector<uint8_t>& nmsResult,
              int width,
              int height);

#endif // NMS_H