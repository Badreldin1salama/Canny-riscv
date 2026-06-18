#include "direction_test.h"
#include "direction.h"

// 1. اختبار الحافة الرأسية (Horizontal Gradient -> direction 0)
TEST_F(DirectionTest, VerticalEdge) {
    // تدرج أفقي يعني Gx كبير و Gy صغير (تحقق شرط ay5 < ax * 2)
    std::fill(Gx.begin(), Gx.end(), 100); 
    std::fill(Gy.begin(), Gy.end(), 0);
    
computeDirection(Gx.data(), Gy.data(), dir.data(), width, height);
    
    EXPECT_EQ(dir[0], 0); 
}

// 2. اختبار الحافة الأفقية (Vertical Gradient -> direction 90)
TEST_F(DirectionTest, HorizontalEdge) {
    // تدرج رأسي يعني Gy كبير و Gx صغير (تحقق شرط ay5 > ax * 12)
    std::fill(Gx.begin(), Gx.end(), 0);
    std::fill(Gy.begin(), Gy.end(), 100);
    
    computeDirection(Gx.data(), Gy.data(), dir.data(), width, height); // هنا سنفحص البكسل الأول
    
    EXPECT_EQ(dir[0], 90); 
}

// 3. اختبار الحافة القطرية (Diagonal -> 45 or 135)
TEST_F(DirectionTest, DiagonalEdge) {
    // تدرج متساوٍ (100, 100)
    Gx[0] = 100; Gy[0] = 100; // الإشارة موجبة (يجب أن يعطي 135)
    Gx[1] = 100; Gy[1] = -100; // إشارة مختلفة (يجب أن يعطي 45)
    
    computeDirection(Gx.data(), Gy.data(), dir.data(), width, height);
    
    EXPECT_EQ(dir[0], 135);
    EXPECT_EQ(dir[1], 45);
}
