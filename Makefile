# ==========================================
# 1. Compilers
# ==========================================
CXX_HOST = g++

# ==========================================
# 2. File Definitions
# ==========================================
# Helper to automatically find all .cpp files in the directory
ALL_CPPS = $(wildcard *.cpp)

# Core implementation files (excludes main.cpp, test files, syscalls, and equivalence test)
LIB_SRCS = $(filter-out main.cpp test.cpp equivalence_test.cpp syscalls.cpp %_test.cpp %_rvv.cpp %_vectorized.cpp, $(ALL_CPPS))

# Source files for the main application
MAIN_SRCS = main.cpp $(LIB_SRCS)

# Source files for the Google Test suite (Excludes RISC-V files)
TEST_SRCS = $(filter-out main.cpp equivalence_test.cpp syscalls.cpp %_rvv.cpp %_vectorized.cpp, $(ALL_CPPS))

# Source files for the RVV Equivalence Test specifically
RVV_TEST_SRCS = equivalence_test.cpp threshold.cpp magnitude.cpp threshold_rvv.cpp magnitude_rvv.cpp

# ==========================================
# 3. حركة ذكية: تحديد الكومبايلر والملفات بناءً على البيئة
# ==========================================
ifeq ($(GITHUB_ACTIONS),true)
    # على جيت هاب: نستخدم كومبايلر اللينكس ومندخلش ملف syscalls في الكومبايل خالص
    CXX_RISCV = riscv64-linux-gnu-g++
    RISCV_SRCS = $(MAIN_SRCS)
else
    # على اللاب توب (Bare Metal): نستخدم unknown-elf ولازم نضيف ملف syscalls عشان الـ I/O يشتغل
    CXX_RISCV = riscv64-unknown-elf-g++
    RISCV_SRCS = $(MAIN_SRCS) syscalls.cpp
endif

# ==========================================
# 4. Flags
# ==========================================
# RISC-V Flags for Vector Extension support
RISCV_FLAGS = -march=rv64gcv -O2

# Google Test Flags (Removed -lgtest_main to avoid multiple definition of main)
GTEST_FLAGS = -lgtest -pthread

# ==========================================
# 5. Targets
# ==========================================
# Default target: builds both host and RISC-V binaries
all: host riscv

# Build for the local host machine (Ryzen/x86)
host:
	$(CXX_HOST) $(MAIN_SRCS) -o my_program_host

# Build for the target architecture (RISC-V with Vector Extensions)
# (تم التصحيح هنا لاستخدام RISCV_SRCS بدل MAIN_SRCS)
riscv:
	$(CXX_RISCV) $(RISCV_FLAGS) $(RISCV_SRCS) -o my_program_riscv

# Build the Google Test executable
test:
	$(CXX_HOST) $(TEST_SRCS) $(GTEST_FLAGS) -o my_tests

# ==========================================
# 6. RVV Equivalence Test Targets
# ==========================================
# Build the RVV test
rvv_test:
	$(CXX_RISCV) $(RISCV_FLAGS) $(RVV_TEST_SRCS) -o test_rvv

# Build and run the RVV test on QEMU directly with multiple VLENs
run_rvv: rvv_test
	@echo "--- Testing with VLEN=128 ---"
	qemu-riscv64 -cpu max,vlen=128 ./test_rvv
	@echo "--- Testing with VLEN=256 ---"
	qemu-riscv64 -cpu max,vlen=256 ./test_rvv
	@echo "--- Testing with VLEN=512 ---"
	qemu-riscv64 -cpu max,vlen=512 ./test_rvv

# ==========================================
# 7. The Magic Command: Clean, Build, Run, PNG
# ==========================================
run: clean riscv
	qemu-riscv64 -cpu max,vlen=128 ./my_program_riscv
	python3 convert.py

# Convert raw to png
png:
	python3 convert.py

# Clean output files and executables
clean:
	rm -f my_program_host my_program_riscv my_tests test_rvv
	rm -f output_*.raw output_*.png