# ==========================================
# 1. Directory Layout
# ==========================================
SRC_DIR  = src
INC_DIR  = include
DATA_DIR = data

# ==========================================
# 2. Compilers
# ==========================================
CXX_HOST = g++

# ==========================================
# 3. File Definitions
# ==========================================
# All .cpp files under src/
ALL_CPPS = $(wildcard $(SRC_DIR)/*.cpp)

# All RVV implementation files (src/*_rvv*.cpp)
RVV_SRCS_ALL = $(wildcard $(SRC_DIR)/*_rvv*.cpp)

# Core scalar lib files — strips main, test entry-points, syscalls, RVV, and vectorized variants
LIB_SRCS = $(filter-out \
    $(SRC_DIR)/main.cpp \
    $(SRC_DIR)/test.cpp \
    $(SRC_DIR)/equivalence_test.cpp \
    $(SRC_DIR)/syscalls.cpp \
    %_test.cpp \
    $(RVV_SRCS_ALL) \
    %_vectorized.cpp, \
    $(ALL_CPPS))

# Main application sources (scalar only — safe to run on host or QEMU)
MAIN_SRCS = $(SRC_DIR)/main.cpp $(LIB_SRCS)

# Google Test suite sources (no RVV, no main, no equivalence, no syscalls)
TEST_SRCS = $(filter-out \
    $(SRC_DIR)/main.cpp \
    $(SRC_DIR)/equivalence_test.cpp \
    $(SRC_DIR)/syscalls.cpp \
    $(RVV_SRCS_ALL) \
    %_vectorized.cpp, \
    $(ALL_CPPS))

# RVV equivalence test: scalar lib + RVV kernels + test driver
RVV_TEST_SRCS = $(SRC_DIR)/equivalence_test.cpp $(LIB_SRCS) $(RVV_SRCS_ALL)

# ==========================================
# 4. Compiler / QEMU selection by environment
# ==========================================
ifeq ($(GITHUB_ACTIONS),true)
    # GitHub Actions: linux-gnu toolchain; QEMU needs sysroot
    CXX_RISCV = riscv64-linux-gnu-g++
    RISCV_SRCS = $(MAIN_SRCS) $(RVV_SRCS_ALL)
    QEMU_ENV = -L /usr/riscv64-linux-gnu
else
    # Local bare-metal: unknown-elf toolchain; syscalls.cpp required
    CXX_RISCV = riscv64-unknown-elf-g++
    RISCV_SRCS = $(MAIN_SRCS) $(RVV_SRCS_ALL) $(SRC_DIR)/syscalls.cpp
    QEMU_ENV =
endif

# ==========================================
# 5. Flags
# ==========================================
INCLUDES    = -I $(INC_DIR)
RISCV_FLAGS = -march=rv64gcv -O2 -static $(INCLUDES)
GTEST_FLAGS = -lgtest -pthread

# ==========================================
# 6. Primary Targets
# ==========================================
all: host riscv

# Build scalar version for the local host machine
host:
	$(CXX_HOST) $(INCLUDES) $(MAIN_SRCS) -o my_program_host

# Build scalar RISC-V binary (used by CI and `make run`)
riscv:
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 $(RISCV_SRCS) -o my_program_riscv

# Build RISC-V binary with RVV enabled (for VLEN sweeps)
riscv_rvv:
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=1 $(RISCV_SRCS) -o my_program_riscv_rvv

# Build and run host-side Google Tests
test:
	$(CXX_HOST) $(INCLUDES) $(TEST_SRCS) $(GTEST_FLAGS) -o my_tests

# ==========================================
# 7. RVV Equivalence Test Targets
# ==========================================
rvv_test:
	$(CXX_RISCV) $(RISCV_FLAGS) $(RVV_TEST_SRCS) -o test_rvv

run_rvv: rvv_test
	@echo "--- Testing with VLEN=128 ---"	
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=128 ./test_rvv 256 256
	@echo "--- Testing with VLEN=256 ---"
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=256 ./test_rvv 256 256
	@echo "--- Testing with VLEN=512 ---"
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./test_rvv 256 256

# ==========================================
# 8. Run / Sweep / VLEN
# ==========================================
# Run scalar binary on QEMU and convert output images
run: clean riscv
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./my_program_riscv 256 256
	python3 scripts/convert.py

# Benchmark RVV binary across VLENs and convert images
run_vlen: clean riscv_rvv
	@echo "===== VLEN = 128 ====="
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=128 ./my_program_riscv_rvv 256 256
	@echo ""
	@echo "===== VLEN = 256 ====="
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=256 ./my_program_riscv_rvv 256 256
	@echo ""
	@echo "===== VLEN = 512 ====="
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./my_program_riscv_rvv 256 256
	@echo "=== Converting RAW images to PNG ==="
	python3 scripts/convert.py

# Compiler optimization sweep (scalar, 6 levels)
sweep:	
	@echo "=== O0 ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O0 $(RISCV_SRCS) -o canny_O0
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O0 256 256

	@echo "=== O1 ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O1 $(RISCV_SRCS) -o canny_O1
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O1 256 256

	@echo "=== O2 ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O2 $(RISCV_SRCS) -o canny_O2
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O2 256 256

	@echo "=== O3 ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O3 $(RISCV_SRCS) -o canny_O3
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O3 256 256

	@echo "=== Os ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -Os $(RISCV_SRCS) -o canny_Os
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_Os 256 256

	@echo "=== Ofast ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -Ofast $(RISCV_SRCS) -o canny_Ofast	
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_Ofast 256 256

# Convert output RAW files to PNG
png:
	python3 scripts/convert.py

# Remove all build artifacts and generated images
clean:
	rm -f my_program_host my_program_riscv my_program_riscv_rvv my_tests test_rvv \
	      canny_O0 canny_O1 canny_O2 canny_O3 canny_Os canny_Ofast
	rm -f output_*.raw output_*.png