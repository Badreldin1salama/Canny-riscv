#include "gaussian_test.h"
#include "gaussian.h" // تضمين الموديول الأصلي

// 1. اختبار الصورة الرمادية الموحدة (Uniform Image)
TEST_F(GaussianTest, UniformImage) {
    std::fill(input.begin(), input.end(), 128); // كل البكسلات 128
    
    applyGaussianBlur(input.data(), output.data(), width, height);

    // فحص البكسلات الداخلية فقط (لتجنب تأثير أصفار الحواف)
    for (int row = 2; row < height - 2; ++row) {
        for (int col = 2; col < width - 2; ++col) {
            EXPECT_NEAR(output[row * width + col], 128, 1); // السماح بفرق ±1
        }
    }
}

// 2. اختبار الصورة السوداء (All Black)
TEST_F(GaussianTest, AllBlackImage) {
    // الـ SetUp قام بجعلها كلها أصفار مسبقاً
applyGaussianBlur(input.data(), output.data(), width, height);

    for (int i = 0; i < width * height; ++i) {
        EXPECT_EQ(output[i], 0);
    }
}

// 3. اختبار النقطة المضيئة (Impulse Response)
TEST_F(GaussianTest, ImpulseSpread) {
    // وضع نقطة بيضاء ناصعة في منتصف الصورة
    int center = (height / 2) * width + (width / 2);
    input[center] = 255;

applyGaussianBlur(input.data(), output.data(), width, height);

    // التأكد من أن النقطة في المنتصف أصبحت أقل من 255 (توزعت)
    EXPECT_LT(output[center], 255);
    EXPECT_GT(output[center], 0);
    
    // التأكد من أن جيرانها أضاءوا قليلاً (اكتسبوا من النقطة المركزية)
    EXPECT_GT(output[center + 1], 0); // الجار الأيمن
    EXPECT_GT(output[center - 1], 0); // الجار الأيسر
}
