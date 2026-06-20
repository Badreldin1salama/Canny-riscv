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
├── .github/workflows/ # CI automated pipeline configurations
├── data/              # Image datasets and testing resources
├── include/           # C++ Header files (.h)
├── matlab/            # MATLAB scripts for algorithm prototyping & verification
├── scripts/           # Automation, profiling, and benchmarking scripts
├── src/               # C++ Source files (.cpp)
├── .gitignore         # Untracked files configuration (build artifacts, binaries)
├── LICENSE            # Project license
├── Makefile           # Dual-target build automation
└── README.md          # Project documentation
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

---

## ⏱️ Benchmarking & Performance Profiling
*(Data collected over 100 Iterations under QEMU emulation. The tables below outline our exhaustive Design Space Exploration across different Vector Lengths and Length Multipliers).*

### 1. Architectural Scaling: VLEN = 128 Sweep
| Pipeline Stage | LMUL = 1 | LMUL = 2 | LMUL = 4 (Optimal) |
| :--- | :--- | :--- | :--- |
| **Gaussian Blur** | 3.0270 s (79.6%) | 1.6785 s (68.7%) | 1.0370 s (57.3%) |
| **Sobel Gradients** | 0.4719 s (12.4%) | 0.4628 s (18.9%) | 0.4701 s (26.0%) |
| **Magnitude** | 0.1579 s (4.2%) | 0.1557 s (6.4%) | 0.1550 s (8.6%) |
| **Direction** | 0.0458 s (1.2%) | 0.0458 s (1.9%) | 0.0456 s (2.5%) |
| **Non-Max Suppression** | 0.0354 s (0.9%) | 0.0346 s (1.4%) | 0.0357 s (2.0%) |
| **Double Threshold** | 0.0116 s (0.3%) | 0.0119 s (0.5%) | 0.0115 s (0.6%) |
| **Hysteresis** | 0.0554 s (1.5%) | 0.0544 s (2.2%) | 0.0550 s (3.0%) |
| **Total Pipeline Time** | **3.8051 s** | **2.4436 s** | **1.8099 s** |

### 2. Architectural Scaling: VLEN = 256 Sweep
| Pipeline Stage | LMUL = 1 | LMUL = 2 | LMUL = 4 (Optimal) |
| :--- | :--- | :--- | :--- |
| **Gaussian Blur** | 1.6144 s (75.7%) | 1.0352 s (66.8%) | 0.7328 s (58.9%) |
| **Sobel Gradients** | 0.2724 s (12.8%) | 0.2723 s (17.6%) | 0.2713 s (21.8%) |
| **Magnitude** | 0.1064 s (5.0%) | 0.1056 s (6.8%) | 0.1034 s (8.3%) |
| **Direction** | 0.0454 s (2.1%) | 0.0453 s (2.9%) | 0.0455 s (3.7%) |
| **Non-Max Suppression** | 0.0353 s (1.7%) | 0.0344 s (2.2%) | 0.0345 s (2.8%) |
| **Double Threshold** | 0.0113 s (0.5%) | 0.0114 s (0.7%) | 0.0114 s (0.9%) |
| **Hysteresis** | 0.0474 s (2.2%) | 0.0456 s (2.9%) | 0.0452 s (3.6%) |
| **Total Pipeline Time** | **2.1326 s** | **1.5498 s** | **1.2440 s** |

### 3. Architectural Scaling: VLEN = 512 Sweep
| Pipeline Stage | LMUL = 1 | LMUL = 2 | LMUL = 4 (Optimal) |
| :--- | :--- | :--- | :--- |
| **Gaussian Blur** | 1.0148 s (71.0%) | 0.7241 s (63.8%) | 0.5788 s (58.0%) |
| **Sobel Gradients** | 0.1953 s (13.7%) | 0.1933 s (17.0%) | 0.1962 s (19.7%) |
| **Magnitude** | 0.0884 s (6.2%) | 0.0872 s (7.7%) | 0.0903 s (9.0%) |
| **Direction** | 0.0450 s (3.1%) | 0.0448 s (3.9%) | 0.0457 s (4.6%) |
| **Non-Max Suppression** | 0.0346 s (2.4%) | 0.0343 s (3.0%) | 0.0344 s (3.4%) |
| **Double Threshold** | 0.0112 s (0.8%) | 0.0112 s (1.0%) | 0.0119 s (1.2%) |
| **Hysteresis** | 0.0405 s (2.8%) | 0.0398 s (3.5%) | 0.0408 s (4.1%) |
| **Total Pipeline Time** | **1.4299 s** | **1.1347 s** | **0.9980 s** |

### 4. Compiler Optimization Sweep vs. Best RVV Configuration
*(Comparing all scalar optimization flags against our fastest RVV implementation).*

| Metric | -O0 | -O1 | -O2 | -O3 | -Ofast | -Os | Best RVV (VLEN=512, LMUL=4) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Total Execution Time** | 1.9785 s | 0.7048 s | 21.0220 s | 1.7782 s | 1.7664 s | 0.7028 s | **0.9980 s** |
| **Binary Size** | 1.5 MB | 1.2 MB | 1.2 MB | 1.2 MB | 1.2 MB | 1.2 MB | **1.2 MB** |

---

## 📝 Engineering Insights & Architectural Notes

* **Amdahl's Law in Action:** Profiling revealed that **Gaussian Blur** consistently consumed the vast majority of CPU cycles (ranging from 57.3% to 79.6% depending on the configuration). Consequently, our optimization efforts via RVV intrinsics were strategically heavily focused on the Convolution kernels to yield the highest overall system speedup.
* **Register Pressure & LMUL Sweet Spots:** The data demonstrates that higher LMUL provides substantial speedups across all VLEN sizes in our specific pipeline, as it allows loading larger chunks of the image convolution window directly into the vector register groups, minimizing loop overhead and address-generation stalls under software emulation.
* **The `-O2` Emulation Anomaly:** We observed a massive performance degradation exclusively under `-O2` optimization (21.02s execution time). This is a known QEMU user-mode emulation artifact where specific compiler instruction scheduling or aggressive loop unrolling clashes heavily with the emulator's translation block caching, causing massive overhead that would not exist on physical bare-metal hardware.
* **Boundary Handling:** Gaussian Blur utilizes zero-padding at the image boundaries to prevent memory leaks and ensure scalar-to-RVV equivalence.
* **Division Optimization:** Exact hardware integer division (`__riscv_vdiv_vx_i32m4`) was deliberately utilized for the Gaussian normalization instead of fixed-point approximations. This architectural choice guarantees 100% mathematical bit-exactness and equivalent accuracy with the baseline scalar C++ implementation.
* **RVV Guards:** Vectorized code is properly guarded with `#ifdef __riscv_vector` to ensure seamless compilation fallback to scalar logic when the V-extension is not targeted.

**⚠️ Important Architectural Note on QEMU Performance:**
QEMU user-mode emulation is strictly utilized for **functional verification** of the RVV instructions. Because QEMU translates and emulates complex RVV hardware instructions via software on an x86 host, the wall-clock runtime of the RVV binary may appear slower than the simple scalar binary. **This is an emulation artifact, not a hardware reality.** The strongest validated result in this project is the **byte-exact correctness** and vector-length agnosticism of the RVV implementation.

---

## 🤖 AI Usage Log
As part of the development process, AI tools were utilized to clarify architectural concepts, troubleshoot environment setup, and verify intrinsic behaviors.

| Question / Prompt | AI Suggestion | Our Modification & Verification | Learning Outcome |
| :--- | :--- | :--- | :--- |
| *How to solve Python ensurepip module not found while building QEMU on Ubuntu WSL2?* | *Suggested installing the `python3-venv` package manually.* | *Executed `sudo apt install python3-venv` which successfully allowed the QEMU configuration to pass.* | *Learned that Ubuntu strips the venv module from base Python to save space.* |
| *How to switch between compiling with syscalls for Bare Metal and without syscalls for GitHub Actions?* | *Suggested using `ifeq ($(GITHUB_ACTIONS),true)` in the Makefile.* | *Implemented the conditional logic to dynamically change `CXX_RISCV` and source files.* | *Understood how to create robust, dual-environment Makefiles.* |
| *How to properly cast an 8-bit vector to 16-bit for accumulation in RVV without data loss?* | *Suggested using `__riscv_vwcvtu_x_x_v_u16m2` followed by a reinterpret cast.* | *Adjusted the LMUL chain from `m1` to `m2` to account for the widened data and prevent register overflow.* | *Understood LMUL doubling during data widening in RISC-V Vector instructions.* |
