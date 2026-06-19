# ==========================================
# 1. Compilers
# ==========================================
CXX_HOST = g++

# ==========================================
# 2. File Definitions
# ==========================================
SRC_CPPS     = $(wildcard src/*.cpp)
TEST_CPPS    = $(wildcard tests/*.cpp)
# تم تصحيح النجمة هنا عشان يجيب كل ملفات الـ rvv
RVV_SRCS_ALL = $(wildcard src/*_rvv.cpp)

# استخدام النمط src/%_rvv.cpp لضمان فلترة كل ملفات الفيكتور من السكالر
LIB_SRCS = $(filter-out src/main.cpp src/syscalls.cpp src/%_rvv.cpp, $(SRC_CPPS))

MAIN_SRCS = src/main.cpp $(LIB_SRCS)

TEST_SRCS = $(filter-out tests/equivalence_test.cpp, $(TEST_CPPS)) $(LIB_SRCS)

RVV_TEST_SRCS = tests/equivalence_test.cpp $(LIB_SRCS) $(RVV_SRCS_ALL)

# ==========================================
# 3. Environment Detection
# ==========================================
ifeq ($(GITHUB_ACTIONS),true)
    CXX_RISCV  = riscv64-linux-gnu-g++
    RISCV_SRCS = $(MAIN_SRCS) $(RVV_SRCS_ALL)
    QEMU_ENV   = -L /usr/riscv64-linux-gnu
else
    CXX_RISCV  = riscv64-unknown-elf-g++
    RISCV_SRCS = $(MAIN_SRCS) $(RVV_SRCS_ALL) src/syscalls.cpp
    QEMU_ENV   =
endif

# ==========================================
# 4. Flags
# ==========================================
INCLUDES    = -Iinclude
RISCV_FLAGS = -march=rv64gcv -O2 -static $(INCLUDES)
HOST_FLAGS  = $(INCLUDES)
GTEST_FLAGS = -lgtest -pthread

# ==========================================
# 5. Main Targets
# ==========================================
all: host riscv

host:
	$(CXX_HOST) $(HOST_FLAGS) $(MAIN_SRCS) -o my_program_host

riscv:
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 $(RISCV_SRCS) -o my_program_riscv

riscv_rvv:
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=1 $(RISCV_SRCS) -o my_program_riscv_rvv

test:
	$(CXX_HOST) $(HOST_FLAGS) $(TEST_SRCS) $(GTEST_FLAGS) -o my_tests

# ==========================================
# 6. RVV Equivalence Test
# ==========================================
rvv_test:
	$(CXX_RISCV) $(RISCV_FLAGS) $(RVV_TEST_SRCS) -o test_rvv

run_rvv: rvv_test
	@echo "--- Testing with VLEN=128 ---"
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=128 ./test_rvv
	@echo "--- Testing with VLEN=256 ---"
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=256 ./test_rvv
	@echo "--- Testing with VLEN=512 ---"
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./test_rvv

# ==========================================
# 7. Run, Sweep, VLEN
# ==========================================
run: clean riscv
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./my_program_riscv
	python3 scripts/convert.py

run_vlen: clean riscv_rvv
	@echo "===== VLEN = 128 ====="
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=128 ./my_program_riscv_rvv
	@echo "===== VLEN = 256 ====="
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=256 ./my_program_riscv_rvv
	@echo "===== VLEN = 512 ====="
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./my_program_riscv_rvv

sweep:
	@echo "=== O0 ===" && $(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O0 $(RISCV_SRCS) -o canny_O0
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O0
	@echo "=== O1 ===" && $(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O1 $(RISCV_SRCS) -o canny_O1
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O1
	@echo "=== O2 ===" && $(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O2 $(RISCV_SRCS) -o canny_O2
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O2
	@echo "=== O3 ===" && $(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O3 $(RISCV_SRCS) -o canny_O3
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O3
	@echo "=== Os ===" && $(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -Os $(RISCV_SRCS) -o canny_Os
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_Os
	@echo "=== Ofast ===" && $(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -Ofast $(RISCV_SRCS) -o canny_Ofast
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_Ofast

png:
	python3 scripts/convert.py

clean:
	rm -f my_program_host my_program_riscv my_program_riscv_rvv my_tests test_rvv canny_O0 canny_O1 canny_O2 canny_O3 canny_Os canny_Ofast

	rm -f output_*.raw output_*.png
