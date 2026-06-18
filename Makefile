# Compilers
CXX_HOST = g++
# تم تصحيح اسم الكومبايلر للنسخة الـ Bare Metal
CXX_RISCV = riscv64-unknown-elf-g++ 

# RISC-V Flags for Vector Extension support + O3 Optimization
RISCV_FLAGS = -O3 -march=rv64gcv -mabi=lp64d -Wall

# Google Test Flags + O3 Optimization
HOST_FLAGS = -O3 -Wall
GTEST_FLAGS = -lgtest -lgtest_main -pthread

# Helper to automatically find all .cpp files in the directory
# This will now automatically pick up syscalls.cpp too!
ALL_CPPS = $(wildcard *.cpp)

# Core implementation files (excludes main.cpp and test.cpp)
LIB_SRCS = $(filter-out main.cpp test.cpp, $(ALL_CPPS))

# Source files for the main application
MAIN_SRCS = main.cpp $(LIB_SRCS)

# Source files for the Google Test suite
TEST_SRCS = test.cpp $(LIB_SRCS)

# Default target: builds both host and RISC-V binaries
all: host canny_rv

# Build for the local host machine (Ryzen/x86)
host:
	$(CXX_HOST) $(HOST_FLAGS) $(MAIN_SRCS) -o my_program_host

# Build for the target architecture (Bare Metal RISC-V with Vector Extensions)
canny_rv:
	$(CXX_RISCV) $(RISCV_FLAGS) $(MAIN_SRCS) -o my_program_riscv.elf

# Build the Google Test executable
test:
	$(CXX_HOST) $(HOST_FLAGS) $(TEST_SRCS) $(GTEST_FLAGS) -o my_tests

# ==========================================
# The Magic Command: Clean, Build, Run, PNG
# ==========================================
run: clean canny_rv
	qemu-riscv64 -cpu rv64,v=true,vlen=128 ./my_program_riscv.elf
	python3 convert.py

# Convert raw to png
png:
	python3 convert.py

# Clean output files and executables
clean:
	rm -f my_program_host my_program_riscv.elf my_tests
	rm -f output_*.raw output_*.png