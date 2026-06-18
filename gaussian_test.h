#ifndef GAUSSIAN_TEST_H
#define GAUSSIAN_TEST_H

#include <gtest/gtest.h>
#include <vector>
#include <cstdint>

// إنشاء Test Fixture خاص بالـ Gaussian
class GaussianTest : public ::testing::Test {
protected:
    int width = 10;
    int height = 10;
    std::vector<uint8_t> input;
    std::vector<uint8_t> output;

    // هذه الدالة تعمل أوتوماتيكياً قبل تشغيل أي اختبار
    void SetUp() override {
        // حجز مساحة بحجم الصورة في الذاكرة للـ input والـ output
        input.assign(width * height, 0);
        output.assign(width * height, 0); // تم حل المشكلة هنا
    }
};

#endif // GAUSSIAN_TEST_H