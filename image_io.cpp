#include "image_io.h"
#include <fstream>
#include <iostream>

bool readRawImage(const std::string& filename, std::vector<uint8_t>& image, int width, int height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << " for reading.\n";
        return false;
    }
    
    // Ensure the vector is properly sized
    image.resize(width * height);
    
    // Read the raw bytes directly into the vector's memory
    file.read(reinterpret_cast<char*>(image.data()), image.size());
    return true;
}

bool writeRawImage(const std::string& filename, const std::vector<uint8_t>& image, int width, int height) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return false;
    }
    
    // Write the raw bytes from the vector to the file
    file.write(reinterpret_cast<const char*>(image.data()), image.size());
    return true;
}