#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "image_io.h"
#include "gaussian.h"
#include "sobel.h"       
#include "magnitude.h"   
#include "direction.h"   
#include "nms.h"         // Added header for Non-Maximum Suppression

int main() {
    // Project specific image dimensions
    int width = 256;  
    int height = 256; 

    // I/O File paths
    std::string inputFile = "test_1.raw";
    std::string blurredFile = "output_1_blurred.raw";
    std::string sobelFile = "output_2_sobel.raw";
    std::string nmsFile = "output_3_nms.raw";     // New output file for NMS

    // Vectors to store image data and gradients
    std::vector<uint8_t> inputImage;
    std::vector<uint8_t> blurredImage;
    
    std::vector<int16_t> Gx;
    std::vector<int16_t> Gy;
    std::vector<uint8_t> sobelMagnitude;
    std::vector<uint8_t> sobelDirection;
    std::vector<uint8_t> nmsImage;                // Vector for NMS output

    std::cout << "--- Testing Non-Maximum Suppression (NMS) Feature ---\n";
    
    // 1. Read the input image
    std::cout << "Reading image...\n";
    if (!readRawImage(inputFile, inputImage, width, height)) {
        std::cerr << "Error: Could not read " << inputFile << "\n";
        return -1; 
    }
    
    // 2. Apply Gaussian Blur (Pre-requisite for Sobel)
    std::cout << "Applying Gaussian Blur...\n";
    applyGaussianBlur(inputImage, blurredImage, width, height);
    // Save the blurred image to verify Gaussian output
    std::cout << "Writing blurred image...\n";
    if (!writeRawImage(blurredFile, blurredImage, width, height)) {
        std::cerr << "Error: Could not write " << blurredFile << "\n";
        return -1; 
    }
    // 3. Apply Sobel Filter to get Gx and Gy
    std::cout << "Applying Sobel Filter...\n";
    applySobel(blurredImage, Gx, Gy, width, height);

    // 4. Compute Gradient Magnitude
    std::cout << "Computing Magnitude...\n";
    computeMagnitude(Gx, Gy, sobelMagnitude, width, height);
    
    // Save the magnitude image to verify Sobel output
    std::cout << "Writing Sobel magnitude image...\n";
    if (!writeRawImage(sobelFile, sobelMagnitude, width, height)) {
        std::cerr << "Error: Could not write " << sobelFile << "\n";
        return -1; 
    }

    // 5. Compute Gradient Direction (Needed for NMS)
    std::cout << "Computing Direction...\n";
    computeDirection(Gx, Gy, sobelDirection, width, height);
    
    // 6. Apply Non-Maximum Suppression (NMS)
    std::cout << "Applying Non-Maximum Suppression...\n";
    applyNMS(sobelMagnitude, sobelDirection, nmsImage, width, height);
    
    // 7. Save the NMS image to verify
    std::cout << "Writing NMS image...\n";
    if (!writeRawImage(nmsFile, nmsImage, width, height)) {
        std::cerr << "Error: Could not write " << nmsFile << "\n";
        return -1; 
    }

    std::cout << "✅ Done! NMS applied successfully.\n";

    return 0;
}