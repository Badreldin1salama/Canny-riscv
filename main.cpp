#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "image_io.h"
#include "gaussian.h"
#include "sobel.h"       
#include "magnitude.h"   
#include "direction.h"   
#include "nms.h"         
#include "threshold.h"   // Added header for Double Thresholding

int main() {
    // Project specific image dimensions
    int width = 256;  
    int height = 256; 

    // I/O File paths
    std::string inputFile = "test_1.raw";
    std::string blurredFile = "output_1_blurred.raw";
    std::string sobelFile = "output_2_sobel.raw";
    std::string nmsFile = "output_3_nms.raw";
    std::string thresholdFile = "output_4_threshold.raw"; // Output file for Threshold stage

    // Vectors to store image data and gradients
    std::vector<uint8_t> inputImage;
    std::vector<uint8_t> blurredImage;
    
    std::vector<int16_t> Gx;
    std::vector<int16_t> Gy;
    std::vector<uint8_t> sobelMagnitude;
    std::vector<uint8_t> sobelDirection;
    std::vector<uint8_t> nmsResult;
    std::vector<uint8_t> thresholdResult; // Vector for Threshold output

    std::cout << "--- Testing Double Thresholding Feature ---\n";
    
    // 1. Read the input image
    std::cout << "Reading image...\n";
    if (!readRawImage(inputFile, inputImage, width, height)) {
        std::cerr << "Error: Could not read " << inputFile << "\n";
        return -1; 
    }
    
    // 2. Apply Gaussian Blur
    std::cout << "Applying Gaussian Blur...\n";
    applyGaussianBlur(inputImage, blurredImage, width, height);
    writeRawImage(blurredFile, blurredImage, width, height);

    // 3. Apply Sobel Filter
    std::cout << "Applying Sobel Filter...\n";
    applySobel(blurredImage, Gx, Gy, width, height);

    // 4. Compute Gradient Magnitude
    std::cout << "Computing Magnitude...\n";
    computeMagnitude(Gx, Gy, sobelMagnitude, width, height);
    writeRawImage(sobelFile, sobelMagnitude, width, height); 

    // 5. Compute Gradient Direction
    std::cout << "Computing Direction...\n";
    computeDirection(Gx, Gy, sobelDirection, width, height);

    // 6. Apply Non-Maximum Suppression (NMS)
    std::cout << "Applying Non-Maximum Suppression...\n";
    applyNMS(sobelMagnitude, sobelDirection, nmsResult, width, height);
    writeRawImage(nmsFile, nmsResult, width, height); 

    // 7. Apply Double Thresholding
    std::cout << "Applying Double Thresholding...\n";
    // Using Low Threshold = 10 and High Threshold = 70 as per your configuration
    applyDoubleThreshold(nmsResult, thresholdResult, width, height, 40, 70);
    
    // 8. Save Threshold result to verify
    std::cout << "Writing Threshold image...\n";
    if (!writeRawImage(thresholdFile, thresholdResult, width, height)) {
        std::cerr << "Error: Could not write " << thresholdFile << "\n";
        return -1; 
    }

    std::cout << "✅ Done! Double Thresholding applied and saved successfully.\n";

    return 0;
}