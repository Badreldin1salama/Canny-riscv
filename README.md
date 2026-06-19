# Canny Edge Detection on RISC-V 🚀
![CI](https://github.com/Badreldin1salama/canny-riscv/actions/workflows/ci.yml/badge.svg)

## 📌 Overview
Implementation of the Canny Edge Detection pipeline optimized with RISC-V Vector Extension (RVV) intrinsics. This repository also contains a comprehensive Unit Testing Suite built using the GoogleTest (GTest) framework to ensure mathematical accuracy, memory safety, and algorithmic reliability across the pipeline.

---

## 👥 Team Roles

| Name | Role |
| :--- | :--- |
| **Badreldin Hossam Badreldin** | Infrastructure & CI Engineer |
| **Belal Abdeltawab Mabrouk** | Algorithm Pipeline Developer |
| **Bassam Khodary Mohammed** | QA & Testing Engineer |
| **Mohammed Hany Dwidar** | RVV & Vectorization Engineer |
| **Taha Hesham** | Performance Analyst & Reporter |

---

## 🏗️ Project Structure

```text
canny-riscv/
├── src/          # Source files (C++ implementation)
├── tests/        # Unit tests (GoogleTest)
├── include/      # Header files
├── build_rv/     # RISC-V build output artifacts
├── build_host/   # Host build output artifacts
└── Makefile      # Dual-target build automation
```

---

## ⚙️ Prerequisites
To run this project and the testing suite on your machine, ensure you have:
* **RISC-V GNU Toolchain** (GCC 16+) built with RVV support (`--with-arch=rv64gcv`).
* **QEMU Emulator** 11+ (`riscv64-linux-user`).
* **Host Compilers:** `g++` and `make`.
* **Testing Framework:** GoogleTest (`libgtest-dev`).

---

## 🚀 Build & Execute
Run the following commands in your terminal to compile and execute the project:

```bash
# 1. Clean previous build artifacts
make clean

# 2. Run host-side unit tests (GoogleTest)
make test

# 3. Cross-compile the pipeline for RISC-V
make canny_rv    

# 4. Execute the RISC-V binary on QEMU (Default VLEN=256)
make run         
```

---

## 🔄 Reproducibility Sequence
To reproduce our exact validation environment and results, run the following sequence:

```bash
make clean
make test
make canny_scalar
make sweep
make run_vlen
```

---

## 🧪 Unit Testing Suite Architecture
The testing suite is highly modular, isolating each stage of the Canny pipeline to pinpoint errors and validate expected behaviors.

| Pipeline Stage | Test File | Test Cases & Validations |
| :--- | :--- | :--- |
| **1. Gaussian Blur** | `gaussian_test.cpp` | **UniformImage:** Grayscale image remains unchanged.<br>**AllBlackImage:** Boundary handling and zero-value retention.<br>**ImpulseSpread:** Proper intensity distribution of a single bright pixel. |
| **2. Sobel Operator** | `sobel_test.cpp` | **UniformImage:** Strictly zero gradient.<br>**VerticalEdge:** High horizontal variance (Gx), zero vertical.<br>**HorizontalEdge:** High vertical variance (Gy), zero horizontal. |
| **3. Gradient Magnitude** | `magnitude_test.cpp` | **NonZeroOutput:** Accurate Pythagorean triples ($Gx^2 + Gy^2$).<br>**AllZerosInput:** Zero magnitude for zero-gradient inputs.<br>**VariationOutput:** Multiple distinct gradient pairs tested simultaneously. |
| **4. Edge Direction** | `direction_test.cpp` | **VerticalEdge:** Asserts **0°** direction.<br>**HorizontalEdge:** Asserts **90°** direction.<br>**DiagonalEdge:** Accurate quantization for **45°** and **135°**. |

---

## 🔍 Correctness & VLEN Sweep Verification
Functional correctness is validated by ensuring byte-identical outputs between the Host/Scalar implementation and the RVV implementation across different vector lengths.

| VLEN Configuration | Output Comparison (cmp) | SHA-256 Hash Matching | Result |
| :--- | :--- | :--- | :--- |
| **VLEN = 128** | No differences | `de2f256064a0af797747c2b97505dc0b9f3df0de4f489eac731c23ae9ca9cc31` == `de2f256064a0af797747c2b97505dc0b9f3df0de4f489eac731c23ae9ca9cc31` | ✅ Passed |
| **VLEN = 256** | No differences | `de2f256064a0af797747c2b97505dc0b9f3df0de4f489eac731c23ae9ca9cc31` == `de2f256064a0af797747c2b97505dc0b9f3df0de4f489eac731c23ae9ca9cc31` | ✅ Passed |
| **VLEN = 512** | No differences | `de2f256064a0af797747c2b97505dc0b9f3df0de4f489eac731c23ae9ca9cc31` == `de2f256064a0af797747c2b97505dc0b9f3df0de4f489eac731c23ae9ca9cc31` | ✅ Passed |

**Verification Command Used:** `cmp scalar_output.raw rvv_output.raw`
