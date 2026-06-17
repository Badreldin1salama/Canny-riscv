#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <string>
#include <cstdint>

// دالة قراءة الصورة وحجز ميموري محاذية لها
bool readRawImage(const std::string& filename, uint8_t*& buffer, int width, int height);

// دالة حفظ الصورة على الهارد
bool writeRawImage(const std::string& filename, const uint8_t* buffer, int width, int height);

#endif // IMAGE_IO_H