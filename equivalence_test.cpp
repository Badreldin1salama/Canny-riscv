#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include "magnitude.h"
#include "threshold.h"
#include "rvv_kernels.h"
#include "direction.h"
#include "sobel.h"
#include "gaussian.h"
#include "gaussian_rvv.h"

const int TEST_WIDTH = 100;
const int TEST_HEIGHT = 75;
const int TEST_SIZE = TEST_WIDTH * TEST_HEIGHT;

int main() {
    std::cout << "==========================================\n";
    std::cout << "🚀 Starting RVV Equivalence Tests (Bare-metal)\n";
    std::cout << "==========================================\n";

    // --- 1. اختبار الـ Double Threshold ---
    uint8_t input(TEST_SIZE);
    uint8_t scalar_out(TEST_SIZE, 0);
    uint8_t rvv_out(TEST_SIZE, 0);

    // توليد أرقام عشوائية لـ Threshold
    for (int i = 0; i < TEST_SIZE; ++i) {
        input[i] = std::rand() % 256;
    }

    applyDoubleThreshold(input, scalar_out, TEST_WIDTH, TEST_HEIGHT, 50, 150);
    applyDoubleThreshold_RVV(input, rvv_out, TEST_WIDTH, TEST_HEIGHT, 50, 150);

    // المقارنة باستخدام assert
    for (int i = 0; i < TEST_SIZE; ++i) {
        if (scalar_out[i] != rvv_out[i]) {
            std::cout << "❌ Mismatch in Threshold at index " << i << "\n";
            assert(false); 
        }
    }
    std::cout << "✅ Double Threshold RVV: PASS\n";

    // --- 2. اختبار الـ Magnitude ---
   int16_tgx(TEST_SIZE);
   int16_tgy(TEST_SIZE);
    uint8_t mag_scalar(TEST_SIZE, 0); // مصفوفة لنتيجة الكود العادي
    uint8_t mag_rvv(TEST_SIZE, 0);    // مصفوفة لنتيجة كود المتجهات

    // توليد أرقام عشوائية لـ Sobel Gradients (بين -500 و 500)
    for(int i = 0; i < TEST_SIZE; ++i) {
        gx[i] = (std::rand() % 1001) - 500;
        gy[i] = (std::rand() % 1001) - 500;
    }

    // استدعاء الدالة العادية ودالة المتجهات للمقارنة بينهما
    computeMagnitude(gx, gy, mag_scalar, TEST_WIDTH, TEST_HEIGHT);
    computeMagnitude_RVV(gx, gy, mag_rvv, TEST_WIDTH, TEST_HEIGHT);

    // مقارنة الـ Magnitude للتأكد من تطابق النتائج تماماً بالملّي
    for (int i = 0; i < TEST_SIZE; ++i) {
        if (mag_scalar[i] != mag_rvv[i]) {
            std::cout << "❌ Mismatch in Magnitude at index " << i 
                      << " (Scalar: " << (int)mag_scalar[i] 
                      << ", RVV: " << (int)mag_rvv[i] << ")\n";
            assert(false);
        }
    }
    std::cout << "✅ Magnitude RVV: PASS\n";

// --- 3. Testing Direction ---
uint8_t dir_scalar(TEST_SIZE, 0);
uint8_t dir_rvv(TEST_SIZE, 0);

computeDirection(gx, gy,
                 dir_scalar,
                 TEST_WIDTH,
                 TEST_HEIGHT);

computeDirection_RVV(gx, gy,
                     dir_rvv,
                     TEST_WIDTH,
                     TEST_HEIGHT);

for (int i = 0; i < TEST_SIZE; ++i)
{
    if (dir_scalar[i] != dir_rvv[i])
    {
        std::cout << "❌ Mismatch in Direction at index "
                  << i
                  << " Scalar="
                  << (int)dir_scalar[i]
                  << " RVV="
                  << (int)dir_rvv[i]
                  << "\n";

        assert(false);
    }
}

std::cout << "✅ Direction RVV: PASS\n";
    
// --- 4. Test Sobel ---
uint8_t sobel_input(TEST_SIZE);

for (int i = 0; i < TEST_SIZE; ++i)
{
    sobel_input[i] = std::rand() % 256;
}

std::vector<int16_t> gx_scalar;
std::vector<int16_t> gy_scalar;

std::vector<int16_t> gx_rvv;
std::vector<int16_t> gy_rvv;

applySobel(
    sobel_input,
    gx_scalar,
    gy_scalar,
    TEST_WIDTH,
    TEST_HEIGHT);

applySobel_RVV(
    sobel_input,
    gx_rvv,
    gy_rvv,
    TEST_WIDTH,
    TEST_HEIGHT);

for (int i = 0; i < TEST_SIZE; ++i)
{
    if (gx_scalar[i] != gx_rvv[i])
    {
        std::cout
            << "❌ Sobel Gx mismatch at "
            << i
            << "\n";

        assert(false);
    }

    if (gy_scalar[i] != gy_rvv[i])
    {
        std::cout
            << "❌ Sobel Gy mismatch at "
            << i
            << "\n";

        assert(false);
    }
}

std::cout << "✅ Sobel RVV: PASS\n";

// --- 5. Test Gaussian ---

uint8_t gaussian_scalar;
uint8_t gaussian_rvv;

applyGaussianBlur(
    input,
    gaussian_scalar,
    TEST_WIDTH,
    TEST_HEIGHT);

applyGaussianBlur_RVV(
    input,
    gaussian_rvv,
    TEST_WIDTH,
    TEST_HEIGHT);

for (int i = 0; i < TEST_SIZE ; i++)
{
    if (gaussian_scalar[i] != gaussian_rvv[i])
    {
        std::cout
            << "Gaussian mismatch at "
                << i
                << " scalar="
                << (int)gaussian_scalar[i]
                << " rvv="
                << (int)gaussian_rvv[i]
                << "\n";

        assert(false);
    }
}

std::cout
    << "✅ Gaussian RVV: PASS\n";

    std::cout << "==========================================\n";
    std::cout << "🎉 ALL RVV TESTS PASSED SUCCESSFULLY!\n";
    return 0;
}
