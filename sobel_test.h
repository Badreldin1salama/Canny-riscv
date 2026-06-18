#ifndef SOBEL_TEST_H
#define SOBEL_TEST_H

#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <cmath>

class SobelTest : public ::testing::Test {
protected:
    int width = 10;
    int height = 10;
    std::vector<uint8_t> input;
    std::vector<int16_t> Gx;
    std::vector<int16_t> Gy;

    void SetUp() override {
        // حجز مساحات فعلية في الذاكرة لكل المصفوفات لمنع الـ Crash
        input.assign(width * height, 0);
        Gx.assign(width * height, 0);
        Gy.assign(width * height, 0);
    }
};

#endif // SOBEL_TEST_H