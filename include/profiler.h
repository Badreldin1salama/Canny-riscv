#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <iostream>
#include <iomanip>

// ==========================================
// 1. كلاس التايمر (tic / toc)
// ==========================================
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;

public:
    void tic() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double toc() {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end_time - start_time).count();
    }
};

// ==========================================
// 2. دالة طباعة التقرير بنفس التنسيق بالمللي
// ==========================================
inline void printProfilingReport(int iterations, double t_gaussian, double t_sobel, 
                                 double t_magnitude, double t_direction, double t_nms, 
                                 double t_threshold, double t_hysteresis) {
    
    double t_pure_pipeline = t_gaussian + t_sobel + t_magnitude + t_direction + t_nms + t_threshold + t_hysteresis;

    std::cout << "\n=======================================================\n";
    std::cout << "             PROFILING RESULTS (" << iterations << " Iterations)\n";
    std::cout << "=======================================================\n";
    std::cout << std::left << std::setw(30) << "Stage" << " | " << std::setw(10) << "Time (s)" << " | Break Down\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << std::setw(30) << "Stage 1: Gaussian Blur"       << " | " << std::setw(10) << t_gaussian   << " | " << std::setprecision(1) << (t_gaussian / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 2: Sobel Gradients"     << " | " << std::setw(10) << t_sobel      << " | " << std::setprecision(1) << (t_sobel / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 3a: Magnitude"          << " | " << std::setw(10) << t_magnitude  << " | " << std::setprecision(1) << (t_magnitude / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 3b: Direction"          << " | " << std::setw(10) << t_direction  << " | " << std::setprecision(1) << (t_direction / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 4: Non-Max Suppression" << " | " << std::setw(10) << t_nms        << " | " << std::setprecision(1) << (t_nms / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 5a: Double Threshold"   << " | " << std::setw(10) << t_threshold  << " | " << std::setprecision(1) << (t_threshold / t_pure_pipeline) * 100 << "%\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Stage 5b: Hysteresis Tracing" << " | " << std::setw(10) << t_hysteresis << " | " << std::setprecision(1) << (t_hysteresis / t_pure_pipeline) * 100 << "%\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << std::setprecision(4) << std::setw(30) << "Total Pure Pipeline Time"     << " | " << t_pure_pipeline << " seconds\n";
    std::cout << "=======================================================\n\n";
}

#endif