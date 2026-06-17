#ifndef HYSTERESIS_H
#define HYSTERESIS_H

#include <cstdint>

/**
 * @brief Applies Hysteresis to connect weak edges to strong edges.
 * @param image Input image after Double Thresholding (modified in-place).
 * @param width Image width.
 * @param height Image height.
 */
void applyHysteresis(uint8_t* image, int width, int height);

#endif // HYSTERESIS_H