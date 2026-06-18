# Canny Edge Detection on RISC-V

Implementation of the Canny Edge Detection pipeline optimized 
with RISC-V Vector Extension (RVV) intrinsics.

## Team Roles
| Name | Role |
| Badreldin Hossam Badreldin| Infrastructure & CI Engineer |
| Belal Abdeltawab Mabrouk | Algorithm Pipeline Developer |
| Bassam Khodary Mohammed | QA & Testing Engineer|
|Mohammed Hany Dwidar | RVV & Vectoization Engineer |
|Taha Hesham Eldin Taha| Performance Analyst & Reporter |

## Project Structure
canny-riscv/
├── src/          # source files
├── tests/        # unit tests
├── include/      # header files
├── build_rv/     # RISC-V build output
├── build_host/   # host build output
└── Makefile
## Prerequisites
- RISC-V GNU Toolchain (GCC 16+) with RVV support
- QEMU 11+ (riscv64-linux-user)
- GoogleTest

## Build & Run
```bash
make canny_rv    # cross-compile for RISC-V
make run         # run on QEMU (vlen=256)
make test        # run host-side unit tests
make clean       # remove build artifacts
```
# 🧪 Canny Edge Detection - Unit Testing Suite

![C++](https://img.shields.io/badge/C++-14%2B-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![Testing](https://img.shields.io/badge/framework-Google_Test-orange.svg)
![Platform](https://img.shields.io/badge/platform-RISC--V%20%7C%20Host-lightgrey.svg)

## 📌 Overview
This repository contains a comprehensive Unit Testing Suite for the Canny Edge Detection pipeline, originally targeted for the RISC-V architecture. The tests are built using the **Google Test (GTest)** framework to ensure the mathematical accuracy, memory safety, and algorithmic reliability of each individual image processing module.

---

## 🏗️ Test Architecture

The testing suite is highly modular, isolating each stage of the Canny pipeline to pinpoint errors and validate expected behaviors:

### 1. Gaussian Blur (`gaussian_test.cpp`)
Validates the spatial smoothing and noise reduction matrix operations.
* **`UniformImage`**: Asserts that a uniform grayscale image remains completely unchanged.
* **`AllBlackImage`**: Verifies boundary handling and zero-value retention to prevent memory leaks.
* **`ImpulseSpread`**: Confirms the proper intensity distribution of a single bright pixel (impulse response) to its immediate neighbors.

### 2. Sobel Operator (`sobel_test.cpp`)
Tests the convolution kernels responsible for gradient calculation across the X and Y axes ($G_x$ and $G_y$).
* **`UniformImage`**: Ensures a strictly zero gradient in areas of constant intensity.
* **`VerticalEdge`**: Validates high horizontal variance ($G_x$) and zero vertical variance across sharp vertical boundaries.
* **`HorizontalEdge`**: Validates high vertical variance ($G_y$) and zero horizontal variance across sharp horizontal boundaries.

### 3. Gradient Magnitude (`magnitude_test.cpp`)
Verifies the edge strength computation utilizing the Euclidean distance formula: $\sqrt{G_x^2 + G_y^2}$.
* **`NonZeroOutput`**: Tests precise Pythagorean triples (e.g., $30, 40 \rightarrow 50$) to guarantee mathematical accuracy.
* **`AllZerosInput`**: Asserts zero magnitude for zero-gradient inputs.
* **`VariationOutput`**: Tests multiple distinct gradient pairs simultaneously across the simulated image array.

### 4. Edge Direction (`direction_test.cpp`)
Ensures accurate angle quantization ($0^\circ, 45^\circ, 90^\circ, 135^\circ$) required for the Non-Maximum Suppression phase.
* **`VerticalEdge`**: Asserts a $0^\circ$ direction for purely vertical structures.
* **`HorizontalEdge`**: Asserts a $90^\circ$ direction for purely horizontal structures.
* **`DiagonalEdge`**: Verifies accurate quantization for both $45^\circ$ and $135^\circ$ diagonal gradients.

---

## 🚀 Getting Started

### Prerequisites
To run these tests on your host machine, ensure you have the following installed:
* `g++` (GNU C++ Compiler)
* `make` (Build automation tool)
* `libgtest-dev` (Google Test Framework)

### Build and Execute
Run the following commands in your terminal to compile and execute the test suite:

```bash
# 1. Clean previous build artifacts
make clean

# 2. Compile the testing suite
make test

# 3. Execute the tests
./my_tests

## CI Status
![CI](https://github.com/Badreldin1salama/canny-riscv/actions/workflows/ci.yml/badge.svg)
