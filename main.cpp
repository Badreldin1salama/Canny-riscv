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

int main() {
    int width = 256;  
    int height = 256; 

    std::string inputFile = "test_1.raw";
    std::string blurredFile = "output_1_blurred.raw";
    std::string sobelFile = "output_2_sobel.raw";
    std::string nmsFile = "output_3_nms.raw";
    std::string thresholdFile = "output_4_threshold.raw"; 
    std::string finalFile = "output_5_final.raw"; 

    std::vector<uint8_t> inputImage;
    std::vector<uint8_t> blurredImage;
    
    std::vector<int16_t> Gx;
    std::vector<int16_t> Gy;
    std::vector<uint8_t> sobelMagnitude;
    std::vector<uint8_t> sobelDirection;
    
    std::vector<uint8_t> nmsResult;
    std::vector<uint8_t> thresholdResult;

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

    return 0;
}