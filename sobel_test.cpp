#include "sobel_test.h"
#include "sobel.h"

// 1. اختبار الصورة السادة (Zero Gradient)
TEST_F(SobelTest, UniformImage) {
    std::fill(input.begin(), input.end(), 100); // لون موحد
    applySobel(input.data(), Gx.data(), Gy.data(), width, height);

    // يجب أن تكون المشتقة صفر في كل مكان داخلي
    for (int i = width + 1; i < (height - 1) * width - 1; ++i) {
        EXPECT_EQ(Gx[i], 0);
        EXPECT_EQ(Gy[i], 0);
    }
}

// 2. اختبار الحافة الرأسية (Vertical Edge)
TEST_F(SobelTest, VerticalEdge) {
    // نصف الصورة الأيسر أسود (0)، والأيمن أبيض (255)
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            input[row * width + col] = (col >= width / 2) ? 255 : 0;
        }
    }

    applySobel(input.data(), Gx.data(), Gy.data(), width, height);

    int edgeCol = width / 2;
    int centerRow = height / 2;
    int index = centerRow * width + edgeCol;

    // عند نقطة الحافة، يجب أن يكون التغير الأفقي كبيراً جداً، والرأسي يقترب من الصفر
    EXPECT_GT(std::abs(Gx[index]), 100); 
    EXPECT_EQ(Gy[index], 0);
}

// 3. اختبار الحافة الأفقية (Horizontal Edge)
TEST_F(SobelTest, HorizontalEdge) {
    // النصف العلوي أسود، والسفلي أبيض
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            input[row * width + col] = (row >= height / 2) ? 255 : 0;
        }
    }

    applySobel(input.data(), Gx.data(), Gy.data(), width, height);

    int edgeRow = height / 2;
    int centerCol = width / 2;
    int index = edgeRow * width + centerCol;

    // العكس هنا: التغير الرأسي كبير، والأفقي صفر
    EXPECT_EQ(Gx[index], 0);
    EXPECT_GT(std::abs(Gy[index]), 100);
}

// 4. اختبار القيمة الدقيقة للحافة الرأسية
TEST_F(SobelTest, ExactVerticalEdge) {
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            input[row * width + col] = (col >= width / 2) ? 255 : 0;
        }
    }

    applySobel(input.data(), Gx.data(), Gy.data(), width, height);

    int edgeCol = width / 2;
    int centerRow = height / 2;
    int index = centerRow * width + edgeCol;

    EXPECT_EQ(std::abs(Gx[index]), 1020);
    EXPECT_EQ(Gy[index], 0);
}

// 5. اختبار القيمة الدقيقة للحافة الأفقية
TEST_F(SobelTest, ExactHorizontalEdge) {
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            input[row * width + col] = (row >= height / 2) ? 255 : 0;
        }
    }

    applySobel(input.data(), Gx.data(), Gy.data(), width, height);

    int edgeRow = height / 2;
    int centerCol = width / 2;
    int index = edgeRow * width + centerCol;

    EXPECT_EQ(Gx[index], 0);
    EXPECT_EQ(std::abs(Gy[index]), 1020);
}

// 6. اختبار الحافة المائلة (Oblique Edge)
TEST_F(SobelTest, ObliqueEdge) {
    // إنشاء خط قطري
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            input[row * width + col] =
                (col >= row) ? 255 : 0;
        }
    }

    applySobel(input.data(), Gx.data(), Gy.data(), width, height);

    int centerRow = height / 2;
    int centerCol = width / 2;
    int index = centerRow * width + centerCol;

    // في الحافة القطرية يجب أن يظهر تغير في الاتجاهين
    EXPECT_NE(Gx[index], 0);
    EXPECT_NE(Gy[index], 0);
}