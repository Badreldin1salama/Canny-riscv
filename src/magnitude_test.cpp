#include "magnitude_test.h"
#include "magnitude.h"

// 1. اختبار قيم غير صفرية (حساب المقدار مع الـ Normalization)
TEST_F(MagnitudeTest, NonZeroOutput) {
    Gx[5] = 30;
    Gy[5] = 40; 

    computeMagnitude(Gx.data(), Gy.data(), mag.data(), width, height);

    // النتيجة المتوقعة بعد الـ L1 Norm والـ Normalization هي 255
    EXPECT_EQ(mag[5], 255);
}

// 2. اختبار صورة أصفار بالكامل
TEST_F(MagnitudeTest, AllZerosInput) {
    std::fill(Gx.begin(), Gx.end(), 0);
    std::fill(Gy.begin(), Gy.end(), 0);

    computeMagnitude(Gx.data(), Gy.data(), mag.data(), width, height);

    for (int i = 0; i < width * height; ++i) {
        EXPECT_EQ(mag[i], 0);
    }
}

// 3. اختبار القيم المتنوعة
TEST_F(MagnitudeTest, VariationOutput) {
    Gx[0] = 3;  Gy[0] = 4;   
    Gx[1] = 8;  Gy[1] = 6;   
    Gx[2] = 12; Gy[2] = 5;   

    computeMagnitude(Gx.data(), Gy.data(), mag.data(), width, height);

    // القيم المتوقعة الصحيحة بناءً على معادلة الـ Normalization في كودك
    EXPECT_EQ(mag[0], 89);
    EXPECT_EQ(mag[1], 178);
    EXPECT_EQ(mag[2], 216);
}