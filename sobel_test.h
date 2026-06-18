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
        input.assign(width * height, 0);
        Gx.clear();
        Gy.clear();
    }
};

#endif // SOBEL_TEST_H
