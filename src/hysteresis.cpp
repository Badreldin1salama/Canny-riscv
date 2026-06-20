#include "hysteresis.h"
#include <cstdlib> // عشان نستخدم malloc و free

void applyHysteresis(uint8_t* image, int width, int height) {
    int size = width * height;

    // حجز مصفوفة عادية كـ Stack بدل std::vector لتوافق الـ Bare Metal
    int* stack = static_cast<int*>(malloc(size * sizeof(int)));
    int stack_top = 0; // مؤشر بيعرفنا إحنا فين في المكدس

    // 1. ندور على كل الحواف القوية (255) ونحط أماكنها في المكدس
    for (int i = 0; i < size; ++i) {
        if (image[i] == 255) {
            stack[stack_top++] = i;
        }
    }

    // 2. نمشي ورا كل حافة قوية ونوصلها بأي حافة ضعيفة (128) جارتها
    int offsets[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    while (stack_top > 0) {
        // نسحب آخر عنصر من المكدس
        int idx = stack[--stack_top];

        int row = idx / width;
        int col = idx % width;

        // فحص الجيران التمانية
        for (int k = 0; k < 8; ++k) {
            int r = row + offsets[k][0];
            int c = col + offsets[k][1];

            if (r >= 0 && r < height && c >= 0 && c < width) {
                int nIdx = r * width + c;
                if (image[nIdx] == 128) { 
                    image[nIdx] = 255;    // نورها أبيض
                    stack[stack_top++] = nIdx; // ضيفها للمكدس عشان نفحص جيرانها هي كمان
                }
            }
        }
    }

    // 3. أي حافة ضعيفة (128) اتبقت ومحدش وصلها، نمسحها ونخليها أسود (0)
    for (int i = 0; i < size; ++i) {
        if (image[i] == 128) {
            image[i] = 0;
        }
    }

    // تنظيف الميموري الخاصة بالمكدس
    free(stack);
}