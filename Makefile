# Compilers
CXX_HOST = g++
CXX_RISCV = riscv64-linux-gnu-g++ 
# RISC-V Flags for Vector Extension support
RISCV_FLAGS = -march=rv64gcv -mabi=lp64d

# Google Test Flags
GTEST_FLAGS = -lgtest -lgtest_main -pthread

# Helper to automatically find all .cpp files in the directory
ALL_CPPS = $(wildcard *.cpp)

# Core implementation files (excludes main.cpp and test.cpp)
# Any new file you add (e.g., sobel.cpp, nms.cpp, otsu.cpp) is automatically included here
LIB_SRCS = $(filter-out main.cpp test.cpp, $(ALL_CPPS))

# Source files for the main application
MAIN_SRCS = main.cpp $(LIB_SRCS)

# Source files for the Google Test suite
TEST_SRCS = test.cpp $(LIB_SRCS)

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
# The Magic Command: Clean, Build, Run, PNG
# ==========================================
run: clean riscv
	qemu-riscv64 -L /usr/riscv64-linux-gnu -cpu rv64,v=true,vlen=128 ./my_program_riscv
	python3 convert.py

# Convert raw to png
png:
	python3 convert.py

# Clean output files and executables
clean:
	rm -f my_program_host my_program_riscv my_tests
	rm -f output_*.raw output_*.png