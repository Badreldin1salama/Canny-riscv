#ifndef RVV_KERNELS_H
#define RVV_KERNELS_H

#include <vector>
#include <cstdint>
#include <riscv_vector.h> // مكتبة الـ RVV الأساسية

void computeDirection_RVV(
    const int16_t* Gx,
    const int16_t* Gy,
    uint8_t* direction,
    int width,
    int height);
// RVV Double Threshold

void applySobel_RVV(
    const uint8_t* inputImage,
    int16_t* Gx,
    int16_t* Gy,
    int width,
    int height);

void applyGaussianBlur_RVV(
    const uint8_t* inputImage,
    uint8_t* outputImage,
    int width,
    int height);

void applyDoubleThreshold_RVV(const uint8_t* nmsImage,
                              uint8_t* thresholdResult,
                              int width, int height,
                              uint8_t lowThreshold, uint8_t highThreshold);

// RVV Magnitude (L1 Norm)
void computeMagnitude_RVV(const int16_t* Gx,
                          const int16_t* Gy,
                          uint8_t* mag,
                          int width, int height);

#endif // RVV_KERNELS_H
