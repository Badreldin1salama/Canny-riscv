#ifndef DIRECTION_TEST_H
#define DIRECTION_TEST_H

#include <gtest/gtest.h>
#include <vector>
#include <cstdint>

class DirectionTest : public ::testing::Test {
protected:
    int width = 4;
    int height = 4;
    std::vector<int16_t> Gx;
    std::vector<int16_t> Gy;
    std::vector<uint8_t> dir;

    void SetUp() override {
        Gx.assign(width * height, 0);
        Gy.assign(width * height, 0);
        dir.assign(width * height, 0);
    }
};

#endif
