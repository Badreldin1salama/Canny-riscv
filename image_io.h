#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <vector>
#include <string>
#include <cstdint>

// Reads a raw grayscale image from disk
bool readRawImage(const std::string& filename, std::vector<uint8_t>& image, int width, int height);

// Writes a raw grayscale image to disk
bool writeRawImage(const std::string& filename, const std::vector<uint8_t>& image, int width, int height);

#endif // IMAGE_IO_H