#include <iostream>
#include <cstdint>
#include <string>
#include <chrono>   // عشان نحسب الوقت بدقة
#include <iomanip>  // عشان تنسيق الجدول
#include <cstdlib>  // عشان aligned_alloc و free

#include "gaussian.h"
#include "sobel.h"
#include "magnitude.h"
#include "direction.h"
#include "nms.h"
#include "threshold.h"
#include "hysteresis.h" 
#include "image_io.h"
#include "gaussian_rvv.h"
#include "rvv_kernels.h"

// ========================================================
// 🛠️ BARE-METAL FILE I/O FIX FOR QEMU USER-MODE
// ========================================================
#ifndef USE_RVV
#define USE_RVV 0
#endif

enum LMUL_Mode
{
    LMUL1,
    LMUL2,
    LMUL4
};
// ========================================================

// تم تعديل الـ Parameters لتقبل Pointers بدلاً من std::vector لتتوافق مع تعديل التيم
void runGaussian(
    LMUL_Mode mode,
    const uint8_t* inputImage,
    uint8_t* blurredImage,
    int width,
    int height)
{
    switch(mode)
    {
        case LMUL1:
            applyGaussianBlur_RVV_LMUL1(inputImage, blurredImage, width, height);
            break;
        case LMUL2:
            applyGaussianBlur_RVV_LMUL2(inputImage, blurredImage, width, height);
            break;
        case LMUL4:
            applyGaussianBlur_RVV_LMUL4(inputImage, blurredImage, width, height);
            break;
        default:
            break;
    }
}

int main() {
    int width = 256;  
    int height = 256; 
    const int ITERATIONS = 100; // عدد اللفات لقياس الأداء بدقة

    std::cout << "USE_RVV = " << USE_RVV << "\n";
    std::string inputFile = "test_1.raw";
    std::string blurredFile = "output_1_blurred.raw";
    std::string sobelFile = "output_2_sobel.raw";
    std::string nmsFile = "output_3_nms.raw";
    std::string thresholdFile = "output_4_threshold.raw"; 
    std::string finalFile = "output_5_final.raw"; 

    // 1. حساب الحجم المطلوب لكل نوع (تعديل التيم للمحاذاة)
    size_t num_pixels = width * height;
    size_t aligned_size_8bit = ((num_pixels * sizeof(uint8_t) + 63) / 64) * 64;
    size_t aligned_size_16bit = ((num_pixels * sizeof(int16_t) + 63) / 64) * 64;

    // 2. تعريف المؤشرات وحجز الميموري باستخدام aligned_alloc
    uint8_t* inputImage = nullptr; 
    uint8_t* blurredImage = static_cast<uint8_t*>(aligned_alloc(64, aligned_size_8bit));
    int16_t* Gx = static_cast<int16_t*>(aligned_alloc(64, aligned_size_16bit));
    int16_t* Gy = static_cast<int16_t*>(aligned_alloc(64, aligned_size_16bit));
    uint8_t* sobelMagnitude = static_cast<uint8_t*>(aligned_alloc(64, aligned_size_8bit));
    uint8_t* sobelDirection = static_cast<uint8_t*>(aligned_alloc(64, aligned_size_8bit));
    uint8_t* nmsResult = static_cast<uint8_t*>(aligned_alloc(64, aligned_size_8bit));
    uint8_t* thresholdResult = static_cast<uint8_t*>(aligned_alloc(64, aligned_size_8bit));

    if (!blurredImage || !Gx || !Gy || !sobelMagnitude || !sobelDirection || !nmsResult || !thresholdResult) {
        std::cerr << "Error: Memory allocation failed!\n";
        return -1;
    }

    std::cout << "Reading image...\n";
    if (!readRawImage(inputFile, inputImage, width, height)) {
        std::cerr << "Error: Could not read " << inputFile << "\n";
        return -1; 
    }
    
    double t_gaussian = 0, t_sobel = 0, t_magnitude = 0, t_direction = 0;
    double t_nms = 0, t_threshold = 0, t_hysteresis = 0, t_pure_pipeline = 0;

    std::cout << "Starting Profiling over " << ITERATIONS << " iterations...\n";

#if USE_RVV

    LMUL_Mode modes[] = { LMUL1, LMUL2, LMUL4 };

    for(int m = 0; m < 3; m++)
    {
        LMUL_Mode mode = modes[m];
        t_gaussian = t_sobel = t_magnitude = t_direction = t_nms = t_threshold = t_hysteresis = t_pure_pipeline = 0;

        std::cout << "\n=====================================\n";
        switch(mode)
        {
            case LMUL1: std::cout << "LMUL = 1\n"; break;
            case LMUL2: std::cout << "LMUL = 2\n"; break;
            case LMUL4: std::cout << "LMUL = 4\n"; break;
            default: break;
        }
        std::cout << "=====================================\n";

        for (int i = 0; i < ITERATIONS; ++i)
        {
            auto start = std::chrono::high_resolution_clock::now();
            runGaussian(mode, inputImage, blurredImage, width, height);
            auto end = std::chrono::high_resolution_clock::now();
            t_gaussian += std::chrono::duration<double>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            applySobel_RVV(blurredImage, Gx, Gy, width, height);
            end = std::chrono::high_resolution_clock::now();
            t_sobel += std::chrono::duration<double>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            computeMagnitude_RVV(Gx, Gy, sobelMagnitude, width, height);
            end = std::chrono::high_resolution_clock::now();
            t_magnitude += std::chrono::duration<double>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            computeDirection(Gx, Gy, sobelDirection, width, height);
            end = std::chrono::high_resolution_clock::now();
            t_direction += std::chrono::duration<double>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            applyNMS(sobelMagnitude, sobelDirection, nmsResult, width, height);
            end = std::chrono::high_resolution_clock::now();
            t_nms += std::chrono::duration<double>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            applyDoubleThreshold(nmsResult, thresholdResult, width, height, 40, 80);
            end = std::chrono::high_resolution_clock::now();
            t_threshold += std::chrono::duration<double>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            applyHysteresis(thresholdResult, width, height);
            end = std::chrono::high_resolution_clock::now();
            t_hysteresis += std::chrono::duration<double>(end - start).count();
        }
        t_pure_pipeline = t_gaussian + t_sobel + t_magnitude + t_direction + t_nms + t_threshold + t_hysteresis;

        std::cout << "\n=======================================================\n";
        std::cout << "             PROFILING RESULTS (" << ITERATIONS << " Iterations)\n";
        std::cout << "=======================================================\n";
        std::cout << std::left << std::setw(30) << "Stage" << " | " << std::setw(10) << "Time (s)" << " | Break Down\n";
        std::cout << "-------------------------------------------------------\n";
        std::cout << std::fixed << std::setprecision(4);
        std::cout << std::setw(30) << "Stage 1: Gaussian Blur" << " | " << std::setw(10) << t_gaussian << " | " << std::setprecision(1) << (t_gaussian / t_pure_pipeline) * 100 << "%\n";
        std::cout << std::setprecision(4) << std::setw(30) << "Stage 2: Sobel Gradients" << " | " << std::setw(10) << t_sobel << " | " << std::setprecision(1) << (t_sobel / t_pure_pipeline) * 100 << "%\n";
        std::cout << std::setprecision(4) << std::setw(30) << "Stage 3a: Magnitude" << " | " << std::setw(10) << t_magnitude << " | " << std::setprecision(1) << (t_magnitude / t_pure_pipeline) * 100 << "%\n";
        std::cout << std::setprecision(4) << std::setw(30) << "Stage 3b: Direction" << " | " << std::setw(10) << t_direction << " | " << std::setprecision(1) << (t_direction / t_pure_pipeline) * 100 << "%\n";
        std::cout << std::setprecision(4) << std::setw(30) << "Stage 4: Non-Max Suppression" << " | " << std::setw(10) << t_nms << " | " << std::setprecision(1) << (t_nms / t_pure_pipeline) * 100 << "%\n";
        std::cout << std::setprecision(4) << std::setw(30) << "Stage 5a: Double Threshold" << " | " << std::setw(10) << t_threshold << " | " << std::setprecision(1) << (t_threshold / t_pure_pipeline) * 100 << "%\n";
        std::cout << std::setprecision(4) << std::setw(30) << "Stage 5b: Hysteresis Tracing" << " | " << std::setw(10) << t_hysteresis << " | " << std::setprecision(1) << (t_hysteresis / t_pure_pipeline) * 100 << "%\n";
        std::cout << "-------------------------------------------------------\n";
        std::cout << std::setprecision(4) << std::setw(30) << "Total Pure Pipeline Time" << " | " << t_pure_pipeline << " seconds\n";
        std::cout << "=======================================================\n\n";
    }

#else

    for (int i = 0; i < ITERATIONS; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        applyGaussianBlur(inputImage, blurredImage, width, height);
        auto end = std::chrono::high_resolution_clock::now();
        t_gaussian += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        applySobel(blurredImage, Gx, Gy, width, height);
        end = std::chrono::high_resolution_clock::now();
        t_sobel += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        computeMagnitude(Gx, Gy, sobelMagnitude, width, height);
        end = std::chrono::high_resolution_clock::now();
        t_magnitude += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        computeDirection(Gx, Gy, sobelDirection, width, height);
        end = std::chrono::high_resolution_clock::now();
        t_direction += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        applyNMS(sobelMagnitude, sobelDirection, nmsResult, width, height);
        end = std::chrono::high_resolution_clock::now();
        t_nms += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        applyDoubleThreshold(nmsResult, thresholdResult, width, height, 40, 80);
        end = std::chrono::high_resolution_clock::now();
        t_threshold += std::chrono::duration<double>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        applyHysteresis(thresholdResult, width, height);
        end = std::chrono::high_resolution_clock::now();
        t_hysteresis += std::chrono::duration<double>(end - start).count();
    }

    t_pure_pipeline = t_gaussian + t_sobel + t_magnitude + t_direction + t_nms + t_threshold + t_hysteresis;

    std::cout << "\n=======================================================\n";
    std::cout << "             PROFILING RESULTS (" << ITERATIONS << " Iterations)\n";
    std::cout << "=======================================================\n";
    std::cout << std::left << std::setw(30) << "Stage" << " | " << std::setw(10) << "Time (s)" << " | Break Down\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << std::setw(30) << "Stage 1: Gaussian Blur" << " | " << std::setw(10) << t_gaussian << " | " << std::setprecision(1) << (t_gaussian / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 2: Sobel Gradients" << " | " << std::setw(10) << t_sobel << " | " << std::setprecision(1) << (t_sobel / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 3a: Magnitude" << " | " << std::setw(10) << t_magnitude << " | " << std::setprecision(1) << (t_magnitude / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 3b: Direction" << " | " << std::setw(10) << t_direction << " | " << std::setprecision(1) << (t_direction / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 4: Non-Max Suppression" << " | " << std::setw(10) << t_nms << " | " << std::setprecision(1) << (t_nms / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 5a: Double Threshold" << " | " << std::setw(10) << t_threshold << " | " << std::setprecision(1) << (t_threshold / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 5b: Hysteresis Tracing" << " | " << std::setw(10) << t_hysteresis << " | " << std::setprecision(1) << (t_hysteresis / t_pure_pipeline) * 100 << "%\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Total Pure Pipeline Time" << " | " << t_pure_pipeline << " seconds\n";
    std::cout << "=======================================================\n\n";

#endif

    // كتابة الصور النهائية بناءً على آخر لفة في الدوران
    std::cout << "Saving Final images...\n";
    writeRawImage(blurredFile, blurredImage, width, height);
    writeRawImage(sobelFile, sobelMagnitude, width, height);
    writeRawImage(nmsFile, nmsResult, width, height);
    writeRawImage(thresholdFile, thresholdResult, width, height);
    writeRawImage(finalFile, thresholdResult, width, height);

    std::cout << "Done! All steps completed.\n";

    // --- Clean up Memory (تعديل التيم عشان مفيش Memory Leak) ---
    free(inputImage); free(blurredImage); free(Gx); free(Gy); 
    free(sobelMagnitude); free(sobelDirection); free(nmsResult); free(thresholdResult);

    return 0;
}