#include "magnitude_test.h"
#include "magnitude.h"

// 1. اختبار قيم غير صفرية (حساب المقدار)
TEST_F(MagnitudeTest, NonZeroOutput) {
    // هنغير قيم بكسل معين عشان نختبر (30، 40 -> المفروض تطلع 50)
    Gx[5] = 30;
    Gy[5] = 40; 

    // بنبعت الـ raw pointers باستخدام .data() عشان تناسب تعريف الدالة
    computeMagnitude(Gx.data(), Gy.data(), mag.data(), width, height);

    // النتيجة المتوقعة: جذر(30^2 + 40^2) = 50
    EXPECT_EQ(mag[5], 50);
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

// 3. اختبار القيم المتنوعة (مثلثات فيثاغورس الشهيرة)
TEST_F(MagnitudeTest, VariationOutput) {
    Gx[0] = 3;  Gy[0] = 4;   // جذر(9+16) = 5
    Gx[1] = 8;  Gy[1] = 6;   // جذر(64+36) = 10
    Gx[2] = 12; Gy[2] = 5;   // جذر(144+25) = 13

    computeMagnitude(Gx.data(), Gy.data(), mag.data(), width, height);

    EXPECT_EQ(mag[0], 5);
    EXPECT_EQ(mag[1], 10);
    EXPECT_EQ(mag[2], 13);
}