#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "image_io.h"
#include "gaussian.h" // Added header for Gaussian blur

int main() {
    // Project specific image dimensions
    int width = 256;  
    int height = 256; 

    // I/O File paths
    std::string inputFile = "test_1.raw";
    std::string blurredFile = "output_1_blurred.raw";

    // Vectors to store image data
    std::vector<uint8_t> inputImage;
    std::vector<uint8_t> blurredImage;

    std::cout << "--- Testing Gaussian Blur Feature ---\n";
    
    // 1. Read the input image
    std::cout << "Reading image...\n";
    if (!readRawImage(inputFile, inputImage, width, height)) {
        std::cerr << "Error: Could not read " << inputFile << "\n";
        return -1; 
    }
    
    // 2. Apply the Gaussian blur filter
    std::cout << "Applying Gaussian Blur...\n";
    applyGaussianBlur(inputImage, blurredImage, width, height);

    // 3. Save the resulting image for testing
    std::cout << "Writing blurred image...\n";
    if (!writeRawImage(blurredFile, blurredImage, width, height)) {
        std::cerr << "Error: Could not write " << blurredFile << "\n";
        return -1; 
    }
    
    std::cout << "✅ Done! Gaussian Blur applied and saved successfully.\n";

    return 0;
}