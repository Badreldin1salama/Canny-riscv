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

## CI Status
![CI](https://github.com/Badreldin1salama/canny-riscv/actions/workflows/ci.yml/badge.svg)
