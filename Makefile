# Compilers
CXX_HOST = g++
CXX_RISCV = riscv64-unknown-elf-g++ 

# RISC-V Flags for Vector Extension support
RISCV_FLAGS = -march=rv64gcv -O2

# Google Test Flags
GTEST_FLAGS = -lgtest -lgtest_main -pthread

# Helper to automatically find all .cpp files in the directory
ALL_CPPS = $(wildcard *.cpp)

# Core implementation files (excludes main.cpp, test.cpp, and equivalence_test.cpp)
# Any new file you add (e.g., sobel.cpp, nms.cpp) is automatically included here
LIB_SRCS = $(filter-out main.cpp test.cpp equivalence_test.cpp, $(ALL_CPPS))

# Source files for the main application
MAIN_SRCS = main.cpp $(LIB_SRCS)

# Source files for the Google Test suite
TEST_SRCS = test.cpp $(LIB_SRCS)

# Source files for the RVV Equivalence Test specifically
RVV_TEST_SRCS = equivalence_test.cpp threshold.cpp magnitude.cpp threshold_rvv.cpp magnitude_rvv.cpp
# Default target: builds both host and RISC-V binaries
all: host riscv

# Build for the local host machine (Ryzen/x86)
host:
	$(CXX_HOST) $(MAIN_SRCS) -o my_program_host

# Build for the target architecture (RISC-V with Vector Extensions)
riscv:
	$(CXX_RISCV) $(RISCV_FLAGS) $(MAIN_SRCS) -o my_program_riscv

# Build the Google Test executable
test:
	$(CXX_HOST) $(TEST_SRCS) $(GTEST_FLAGS) -o my_tests

# ==========================================
# RVV Equivalence Test Targets
# ==========================================
# Build the RVV test
rvv_test:
	$(CXX_RISCV) $(RISCV_FLAGS) $(RVV_TEST_SRCS) -o test_rvv

# Build and run the RVV test on QEMU directly
# Build and run the RVV test on QEMU directly with multiple VLENs
run_rvv: rvv_test
	@echo "--- Testing with VLEN=128 ---"
	qemu-riscv64 -cpu max,vlen=128 ./test_rvv
	@echo "--- Testing with VLEN=256 ---"
	qemu-riscv64 -cpu max,vlen=256 ./test_rvv
	@echo "--- Testing with VLEN=512 ---"
	qemu-riscv64 -cpu max,vlen=512 ./test_rvv
# The Magic Command: Clean, Build, Run, PNG
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
