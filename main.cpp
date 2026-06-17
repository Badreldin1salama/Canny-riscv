#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "gaussian.h"
#include "sobel.h"
#include "magnitude.h"
#include "direction.h"
#include "nms.h"
#include "threshold.h"
#include "hysteresis.h" 
#include "image_io.h"
#include <cstdlib>

int main() {
    int width = 256;  
    int height = 256; 

    std::string inputFile = "test_1.raw";
    std::string blurredFile = "output_1_blurred.raw";
    std::string sobelFile = "output_2_sobel.raw";
    std::string nmsFile = "output_3_nms.raw";
    std::string thresholdFile = "output_4_threshold.raw"; 
    std::string finalFile = "output_5_final.raw"; 

   // 1. حساب الحجم المطلوب لكل نوع (مع ضمان المحاذاة لـ 64 بايت)
    size_t num_pixels = width * height;
    size_t aligned_size_8bit = ((num_pixels * sizeof(uint8_t) + 63) / 64) * 64;
    size_t aligned_size_16bit = ((num_pixels * sizeof(int16_t) + 63) / 64) * 64;

    // 2. تعريف المؤشرات وحجز الميموري
    // صورة الإدخال مش هنحجزلها هنا، لأن دالة readRawImage هي اللي هتحجزلها من جوه
    uint8_t* inputImage = nullptr; 

    // باقي المصفوفات هنحجزلها الميموري بتاعتنا بـ aligned_alloc
    uint8_t* blurredImage = static_cast<uint8_t*>(aligned_alloc(64, aligned_size_8bit));
    
    int16_t* Gx = static_cast<int16_t*>(aligned_alloc(64, aligned_size_16bit));
    int16_t* Gy = static_cast<int16_t*>(aligned_alloc(64, aligned_size_16bit));
    
    uint8_t* sobelMagnitude = static_cast<uint8_t*>(aligned_alloc(64, aligned_size_8bit));
    uint8_t* sobelDirection = static_cast<uint8_t*>(aligned_alloc(64, aligned_size_8bit));
    
    uint8_t* nmsResult = static_cast<uint8_t*>(aligned_alloc(64, aligned_size_8bit));
    uint8_t* thresholdResult = static_cast<uint8_t*>(aligned_alloc(64, aligned_size_8bit));

    // 3. التأكد إن الحجز تم بنجاح ومفيش مشاكل في الرامات
    if (!blurredImage || !Gx || !Gy || !sobelMagnitude || !sobelDirection || !nmsResult || !thresholdResult) {
        std::cerr << "Error: Memory allocation failed!\n";
        return -1;
    }

  std::cout << "Reading image...\n";
    if (!readRawImage(inputFile, inputImage, width, height)) return -1; 
    
    std::cout << "Applying Gaussian Blur...\n";
    applyGaussianBlur(inputImage, blurredImage, width, height);
    writeRawImage(blurredFile, blurredImage, width, height);

    std::cout << "Applying Sobel Filter...\n";
    applySobel(blurredImage, Gx, Gy, width, height);

    std::cout << "Computing Magnitude...\n";
    computeMagnitude(Gx, Gy, sobelMagnitude, width, height);
    writeRawImage(sobelFile, sobelMagnitude, width, height); 

    std::cout << "Computing Direction...\n";
    computeDirection(Gx, Gy, sobelDirection, width, height);

    std::cout << "Applying Non-Maximum Suppression...\n";
    applyNMS(sobelMagnitude, sobelDirection, nmsResult, width, height);
    writeRawImage(nmsFile, nmsResult, width, height); 

    std::cout << "Applying Double Thresholding...\n";
    applyDoubleThreshold(nmsResult, thresholdResult, width, height, 40, 80);
    writeRawImage(thresholdFile, thresholdResult, width, height); 

    std::cout << "Applying Hysteresis...\n";
    applyHysteresis(thresholdResult, width, height);
    std::cout << "Saving Final image...\n";
    if (!writeRawImage(finalFile, thresholdResult, width, height)) return -1; 
    
    std::cout << "Done! All steps completed and all images saved successfully.\n";

    // --- Clean up Memory ---
    free(inputImage);
    free(blurredImage);
    free(Gx);
    free(Gy);
    free(sobelMagnitude);
    free(sobelDirection);
    free(nmsResult);
    free(thresholdResult);
    return 0;
}