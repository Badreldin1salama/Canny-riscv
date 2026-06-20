#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cmath> // عشان نستخدم std::abs للتقريب
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
    // التصحيح: استخدام std::vector بدلاً من متغير مفرد
    std::vector<uint8_t> input(TEST_SIZE);
    std::vector<uint8_t> scalar_out(TEST_SIZE, 0);
    std::vector<uint8_t> rvv_out(TEST_SIZE, 0);

    // توليد أرقام عشوائية لـ Threshold
    for (int i = 0; i < TEST_SIZE; ++i) {
        input[i] = std::rand() % 256;
    }

    // تمرير الـ Pointers باستخدام .data()
    applyDoubleThreshold(input.data(), scalar_out.data(), TEST_WIDTH, TEST_HEIGHT, 50, 150);
    applyDoubleThreshold_RVV(input.data(), rvv_out.data(), TEST_WIDTH, TEST_HEIGHT, 50, 150);

    // المقارنة
    for (int i = 0; i < TEST_SIZE; ++i) {
        if (scalar_out[i] != rvv_out[i]) {
            std::cout << "❌ Mismatch in Threshold at index " << i << "\n";
            assert(false); 
        }
    }
    std::cout << "✅ Double Threshold RVV: PASS\n";


    // --- 2. اختبار الـ Magnitude ---
    std::vector<int16_t> gx(TEST_SIZE);
    std::vector<int16_t> gy(TEST_SIZE);
    std::vector<uint8_t> mag_scalar(TEST_SIZE, 0);
    std::vector<uint8_t> mag_rvv(TEST_SIZE, 0);

    for(int i = 0; i < TEST_SIZE; ++i) {
        gx[i] = (std::rand() % 1001) - 500;
        gy[i] = (std::rand() % 1001) - 500;
    }

    computeMagnitude(gx.data(), gy.data(), mag_scalar.data(), TEST_WIDTH, TEST_HEIGHT);
    computeMagnitude_RVV(gx.data(), gy.data(), mag_rvv.data(), TEST_WIDTH, TEST_HEIGHT);

    // المقارنة مع سماحية 1 بسبب التقريب (كما طلب الدكتور)
    for (int i = 0; i < TEST_SIZE; ++i) {
        if (std::abs(mag_scalar[i] - mag_rvv[i]) > 1) {
            std::cout << "❌ Mismatch in Magnitude at index " << i 
                      << " (Scalar: " << (int)mag_scalar[i] 
                      << ", RVV: " << (int)mag_rvv[i] << ")\n";
            assert(false);
        }
    }
    std::cout << "✅ Magnitude RVV: PASS\n";


    // --- 3. Testing Direction ---
    std::vector<uint8_t> dir_scalar(TEST_SIZE, 0);
    std::vector<uint8_t> dir_rvv(TEST_SIZE, 0);

    computeDirection(gx.data(), gy.data(), dir_scalar.data(), TEST_WIDTH, TEST_HEIGHT);
    computeDirection_RVV(gx.data(), gy.data(), dir_rvv.data(), TEST_WIDTH, TEST_HEIGHT);

    for (int i = 0; i < TEST_SIZE; ++i) {
        if (dir_scalar[i] != dir_rvv[i]) {
            std::cout << "❌ Mismatch in Direction at index " << i << "\n";
            assert(false);
        }
    }
    std::cout << "✅ Direction RVV: PASS\n";
    

    // --- 4. Test Sobel ---
    std::vector<uint8_t> sobel_input(TEST_SIZE);
    for (int i = 0; i < TEST_SIZE; ++i) {
        sobel_input[i] = std::rand() % 256;
    }

    // يجب تحديد الحجم للـ Vectors الخاصة بالـ Sobel
    std::vector<int16_t> gx_scalar(TEST_SIZE, 0);
    std::vector<int16_t> gy_scalar(TEST_SIZE, 0);
    std::vector<int16_t> gx_rvv(TEST_SIZE, 0);
    std::vector<int16_t> gy_rvv(TEST_SIZE, 0);

    applySobel(sobel_input.data(), gx_scalar.data(), gy_scalar.data(), TEST_WIDTH, TEST_HEIGHT);
    applySobel_RVV(sobel_input.data(), gx_rvv.data(), gy_rvv.data(), TEST_WIDTH, TEST_HEIGHT);

    for (int i = 0; i < TEST_SIZE; ++i) {
        if (std::abs(gx_scalar[i] - gx_rvv[i]) > 1) {
            std::cout << "❌ Sobel Gx mismatch at " << i << "\n";
            assert(false);
        }
        if (std::abs(gy_scalar[i] - gy_rvv[i]) > 1) {
            std::cout << "❌ Sobel Gy mismatch at " << i << "\n";
            assert(false);
        }
    }
    std::cout << "✅ Sobel RVV: PASS\n";


    // --- 5. Test Gaussian ---
    std::vector<uint8_t> gaussian_scalar(TEST_SIZE, 0);
    std::vector<uint8_t> gaussian_rvv(TEST_SIZE, 0);

    applyGaussianBlur(input.data(), gaussian_scalar.data(), TEST_WIDTH, TEST_HEIGHT);
    applyGaussianBlur_RVV(input.data(), gaussian_rvv.data(), TEST_WIDTH, TEST_HEIGHT);

    for (int i = 0; i < TEST_SIZE; ++i) {
        // سماحية 1 بسبب عمليات القسمة في الفلتر
        if (std::abs(gaussian_scalar[i] - gaussian_rvv[i]) > 1) {
            std::cout << "❌ Gaussian mismatch at " << i 
                      << " scalar=" << (int)gaussian_scalar[i] 
                      << " rvv=" << (int)gaussian_rvv[i] << "\n";
            assert(false);
        }
    }
    std::cout << "✅ Gaussian RVV: PASS\n";

    std::cout << "==========================================\n";
    std::cout << "🎉 ALL RVV TESTS PASSED SUCCESSFULLY!\n";
    return 0;
}