# Compilers
CXX_HOST = g++

# Helper to automatically find all .cpp files
ALL_CPPS = $(wildcard *.cpp)

# استبعاد syscalls.cpp من الملفات الأساسية عشان نتحكم فيه بمزاجنا
LIB_SRCS = $(filter-out main.cpp test.cpp syscalls.cpp, $(ALL_CPPS))

# Source files for the main application (Host)
MAIN_SRCS = main.cpp $(LIB_SRCS)

# Source files for the Google Test suite
TEST_SRCS = test.cpp $(LIB_SRCS)

# ==========================================
# حركة ذكية: تحديد الكومبايلر والملفات بناءً على البيئة
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

# RISC-V Flags for Vector Extension support + O3 Optimization
RISCV_FLAGS = -O3 -march=rv64gcv -mabi=lp64d -Wall
HOST_FLAGS = -O3 -Wall
GTEST_FLAGS = -lgtest -lgtest_main -pthread

# Default target
all: host canny_rv

# Build for the local host machine (Ryzen/x86)
host:
	$(CXX_HOST) $(HOST_FLAGS) $(MAIN_SRCS) -o my_program_host

# Build for the target architecture
canny_rv:
	$(CXX_RISCV) $(RISCV_FLAGS) $(RISCV_SRCS) -o my_program_riscv.elf

# Alias for GitHub Actions
riscv: canny_rv

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