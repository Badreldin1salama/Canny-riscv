#ifndef MAGNITUDE_TEST_H
#define MAGNITUDE_TEST_H

#include <gtest/gtest.h>
#include <vector>
#include <cstdint>

class MagnitudeTest : public ::testing::Test {
protected:
    int width = 4;
    int height = 4;
    std::vector<int16_t> Gx;
    std::vector<int16_t> Gy;
    std::vector<uint8_t> mag;

    void SetUp() override {
        Gx.assign(width * height, 10); // قيم تجريبية
        Gy.assign(width * height, 10);
        mag.assign(width * height, 0);
    }
};

#endif
