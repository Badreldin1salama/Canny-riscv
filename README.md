
````
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
````

## ⚙️ Prerequisites

To run this project and the testing suite on your machine, ensure you have:

- **RISC-V GNU Toolchain** (GCC 16+) built with RVV support (`--with-arch=rv64gcv`).
    
- **QEMU Emulator** 11+ (`riscv64-linux-user`).
    
- **Host Compilers:** `g++` and `make`.
    
- **Testing Framework:** GoogleTest (`libgtest-dev`).
    

## 🚀 Build & Execute

Run the following commands in your terminal to compile and execute the project:

Bash

```
# 1. Clean previous build artifacts
make clean

# 2. Run host-side unit tests (GoogleTest)
make test

# 3. Cross-compile the pipeline for RISC-V
make canny_rv    

# 4. Execute the RISC-V binary on QEMU (Default VLEN=256)
make run         
```

## 🔄 Reproducibility Sequence

To reproduce our exact validation environment and results, run the following sequence:

Bash

```
make clean
make test
make canny_scalar
make sweep
make run_vlen
```
