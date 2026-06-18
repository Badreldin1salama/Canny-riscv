#include "magnitude_test.h"
#include "magnitude.h"

// 1. اختبار أن الـ Magnitude تعطي قيم صحيحة وغير صفرية
TEST_F(MagnitudeTest, NonZeroOutput) {
    // نغير القيم عشان نضمن إن maxMag مش هيكون صفر
    std::fill(Gx.begin(), Gx.end(), 10);
    std::fill(Gy.begin(), Gy.end(), 10);
    
    computeMagnitude(Gx, Gy, mag, width, height);
    
    for (int i = 0; i < width * height; ++i) {
        // بما إن Gx و Gy متساويين، المفروض النتيجة تكون 255 (لأنهم كلهم maxMag)
        EXPECT_EQ(mag[i], 255);
    }
}

// 2. اختبار حالة الأصفار (All zeros input)
TEST_F(MagnitudeTest, AllZerosInput) {
    std::fill(Gx.begin(), Gx.end(), 0);
    std::fill(Gy.begin(), Gy.end(), 0);
    
    computeMagnitude(Gx, Gy, mag, width, height);
    
    // عند وجود أصفار، الـ Magnitude يجب أن يكون صفراً
    for (int i = 0; i < width * height; ++i) {
        EXPECT_EQ(mag[i], 0);
    }
}

// 3. اختبار التدرج (تأكد إن فيه تباين في القيم)
TEST_F(MagnitudeTest, VariationOutput) {
    // وضع قيم مختلفة
    Gx[0] = 10; Gy[0] = 0;
    Gx[1] = 100; Gy[1] = 0;
    
    computeMagnitude(Gx, Gy, mag, width, height);
    
    // القيمة الثانية المفروض تكون أكبر من الأولى
    EXPECT_GT(mag[1], mag[0]);
}
