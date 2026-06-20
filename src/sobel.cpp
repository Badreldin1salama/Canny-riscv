#include "sobel.h"
#include <cstring> // ضروري عشان نستخدم memset

void applySobel(const uint8_t* inputImage,
                int16_t* Gx,
                int16_t* Gy,
                int width, int height) {
    
    int size = width * height;
    
    // تصفير المصفوفات بالكامل لضمان إن بيكسلات الحدود تكون بصفر
    std::memset(Gx, 0, size * sizeof(int16_t));
    std::memset(Gy, 0, size * sizeof(int16_t));

    for (int row = 1; row < height - 1; ++row) {
        for (int col = 1; col < width - 1; ++col) {
            
            int p00 = inputImage[(row - 1) * width + (col - 1)];
            int p01 = inputImage[(row - 1) * width + col];
            int p02 = inputImage[(row - 1) * width + (col + 1)];
            int p10 = inputImage[row * width + (col - 1)];
            // p11 is the center, multiplied by 0
            int p12 = inputImage[row * width + (col + 1)];
            int p20 = inputImage[(row + 1) * width + (col - 1)];
            int p21 = inputImage[(row + 1) * width + col];
            int p22 = inputImage[(row + 1) * width + (col + 1)];

            // Sobel X kernel
            int gx = -p00 + p02 - 2 * p10 + 2 * p12 - p20 + p22;
            
            // Sobel Y kernel
            int gy = -p00 - 2 * p01 - p02 + p20 + 2 * p21 + p22;

            int index = row * width + col;
            Gx[index] = static_cast<int16_t>(gx);
            Gy[index] = static_cast<int16_t>(gy);
        }
    }
}