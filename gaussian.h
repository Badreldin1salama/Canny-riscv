#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <vector>
#include <cstdint>

/**
 * @brief Applies a 2D Gaussian Blur using a 5x5 integer kernel.
 * * Boundary handling: Zero-padding. Out-of-bounds pixels are treated as 0.
 * * Template Parameters:
 * @param TPixel  Pixel type (default uint8_t for grayscale).
 * @param TAccum  Accumulator type (default int32_t to avoid overflow).
 * @param TKernel Kernel coefficient type (default int16_t).
 */
template <typename TPixel = uint8_t, typename TAccum = int32_t, typename TKernel = int16_t>
void applyGaussianBlur(const std::vector<TPixel>& inputImage,
                       std::vector<TPixel>& outputImage,
                       int width,
                       int height) {
    
    outputImage.resize(width * height);

    // 5x5 Gaussian kernel with sigma ~= 1.0
    // Integer coefficients perfectly summing to 273
    const TKernel kernel[5][5] = {
        { 1,  4,  7,  4,  1 },
        { 4, 16, 26, 16,  4 },
        { 7, 26, 41, 26,  7 },
        { 4, 16, 26, 16,  4 },
        { 1,  4,  7,  4,  1 }
    };
    
    const int kernelRadius = 2;
    const TAccum kernelSum = 273;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            
            TAccum sum = 0; // Uses int32_t to safely accumulate without overflow

            for (int kRow = -kernelRadius; kRow <= kernelRadius; ++kRow) {
                for (int kCol = -kernelRadius; kCol <= kernelRadius; ++kCol) {
                    
                    int neighborRow = row + kRow;
                    int neighborCol = col + kCol;

                    // Boundary handling: Zero-padding
                    // Only add to sum if the pixel is STRICTLY inside the image boundaries.
                    // If outside, it acts as a 0 (0 * kernel_weight = 0, so we just skip it).
                    if (neighborRow >= 0 && neighborRow < height &&
                        neighborCol >= 0 && neighborCol < width) {
                        
                        int flatIndex = neighborRow * width + neighborCol;
                        TPixel pixelValue = inputImage[flatIndex];
                        TKernel weight = kernel[kRow + kernelRadius][kCol + kernelRadius];
                        
                        sum += static_cast<TAccum>(pixelValue) * weight;
                    }
                }
            }

            // Divide by 273
            TAccum finalValue = sum / kernelSum;

            // Clamp to [0, 255] just in case, though math shouldn't exceed it
            if (finalValue < 0) finalValue = 0;
            if (finalValue > 255) finalValue = 255;

            outputImage[row * width + col] = static_cast<TPixel>(finalValue);
        }
    }
}

#endif // GAUSSIAN_H