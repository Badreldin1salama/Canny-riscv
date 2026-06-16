#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "image_io.h"

int main() {
    int width = 256;  
    int height = 256; 

    
    std::string inputFile = "test_1.raw";
    std::string outputFile = "output_base_test.raw";

    std::vector<uint8_t> image;

    std::cout << "Testing Base Project Structure...\n";
    
    std::cout << "Reading image...\n";
    if (!readRawImage(inputFile, image, width, height)) {
        std::cerr << "Error: Could not read " << inputFile << "\n";
        return -1; 
    }
    
    std::cout << "Writing image to verify I/O...\n";
    if (!writeRawImage(outputFile, image, width, height)) {
        std::cerr << "Error: Could not write " << outputFile << "\n";
        return -1; 
    }
    
    std::cout << "✅ Done! Base project structure and I/O are working perfectly.\n";

    return 0;
}