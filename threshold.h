#ifndef THRESHOLD_H
#define THRESHOLD_H

#include <vector>
#include <cstdint>

/**
 * @brief Applies Double Thresholding to categorize pixels into strong, weak, and non-edges.
 * @param nmsImage Input image after Non-Maximum Suppression.
 * @param thresholdResult Output image with strong (255), weak (128), and zero pixels.
 * @param width Image width.
 * @param height Image height.
 * @param lowThreshold Values below this are set to 0.
 * @param highThreshold Values above or equal to this are set to 255.
 */
void applyDoubleThreshold(const std::vector<uint8_t>& nmsImage,
                          std::vector<uint8_t>& thresholdResult,
                          int width,
                          int height,
                          uint8_t lowThreshold,
                          uint8_t highThreshold);

#endif // THRESHOLD_H