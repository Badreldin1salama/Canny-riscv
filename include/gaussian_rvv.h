#ifndef GAUSSIAN_RVV_H
#define GAUSSIAN_RVV_H

#include <vector>
#include <cstdint>

void applyGaussianBlur_RVV(
    const uint8_t* inputImage,
    uint8_t* outputImage,
    int width,
    int height);

void applyGaussianBlur_RVV_LMUL1(
    const uint8_t* inputImage,
    uint8_t* outputImage,
    int width,
    int height);

void applyGaussianBlur_RVV_LMUL2(
     const uint8_t* inputImage,
    uint8_t* outputImage,
    int width,
    int height);

void applyGaussianBlur_RVV_LMUL4(
     const uint8_t* inputImage,
    uint8_t* outputImage,
    int width,
    int height);

#endif