#include "image_io.h"
#include <fstream>
#include <iostream>
#include <cstdlib> // ضروري عشان aligned_alloc و free

bool readRawImage(const std::string& filename, uint8_t*& buffer, int width, int height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open file for reading: " << filename << "\n";
        return false;
    }

    size_t size = static_cast<size_t>(width) * height;
    
    // شرط دالة aligned_alloc أن الحجم المحجوز يكون مضاعف لدرجة المحاذاة (64)
    // لـ 256*256 الحجم 65536 وهو يقبل القسمة على 64 فعلياً، ولكن هذه الحسبة تضمن أي حجم مستقبلي
    size_t aligned_size = ((size + 63) / 64) * 64; 

    // حجز ميموري محاذية على 64 بايت كما طلب الدكتور
    buffer = static_cast<uint8_t*>(aligned_alloc(64, aligned_size));
    
    if (buffer == nullptr) {
        std::cerr << "Error: Memory allocation failed for " << filename << "\n";
        return false;
    }

    // قراءة الداتا الصافية وضخها في البافر
    file.read(reinterpret_cast<char*>(buffer), size);
    if (!file) {
        std::cerr << "Error: Could not read full image data from " << filename << "\n";
        free(buffer); // تنظيف الميموري لو القراءة فشلت
        buffer = nullptr;
        return false;
    }

    return true;
}

bool writeRawImage(const std::string& filename, const uint8_t* buffer, int width, int height) {
    if (buffer == nullptr) {
        std::cerr << "Error: Buffer is null, cannot write.\n";
        return false;
    }
    
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open file for writing: " << filename << "\n";
        return false;
    }

    size_t size = static_cast<size_t>(width) * height;
    file.write(reinterpret_cast<const char*>(buffer), size);
    
    return file.good();
}