# ==========================================
# 1. Compilers
# ==========================================
CXX_HOST = g++

# ==========================================
# 2. File Definitions (تحديد تلقائي ذكي للملفات)
# ==========================================
# Helper to automatically find all .cpp files in the directory
ALL_CPPS = $(wildcard *.cpp)

# Core scalar implementation files (ملفات السكالر فقط - استبعاد الـ RVV والتست)
LIB_SRCS = $(filter-out main.cpp test.cpp equivalence_test.cpp syscalls.cpp %_test.cpp %_rvv.cpp %_vectorized.cpp, $(ALL_CPPS))

# Source files for the main application (Scalar only for Host machine)
MAIN_SRCS = main.cpp $(LIB_SRCS)

# Source files for the Google Test suite (Excludes RISC-V vector files)
TEST_SRCS = $(filter-out main.cpp equivalence_test.cpp syscalls.cpp %_rvv.cpp %_vectorized.cpp, $(ALL_CPPS))

# Automatically catch all your RVV implementation files (تجميع تلقائي لملفات الـ RVV بتاعتك)
RVV_SRCS_ALL = $(wildcard *_rvv.cpp)

# Source files for the RVV Equivalence Test (يجمع السكالر مع الفيكتور أوتوماتيك)
RVV_TEST_SRCS = equivalence_test.cpp $(LIB_SRCS) $(RVV_SRCS_ALL)

# ==========================================
# 3. حركة التيم الذكية: تحديد الكومبايلر والملفات والـ QEMU بناءً على البيئة
# ==========================================
ifeq ($(GITHUB_ACTIONS),true)
    # على جيت هاب: نستخدم كومبايلر اللينكس ومندخلش ملف syscalls، وبنحتاج الـ Sysroot path في QEMU
    CXX_RISCV = riscv64-linux-gnu-g++
    RISCV_SRCS = $(MAIN_SRCS) $(RVV_SRCS_ALL)
    QEMU_ENV = -L /usr/riscv64-linux-gnu
else
    # على اللاب توب (Bare Metal): نستخدم unknown-elf ولازم نضيف ملف syscalls والـ QEMU مش محتاج لودر خارجي
    CXX_RISCV = riscv64-unknown-elf-g++
    RISCV_SRCS = $(MAIN_SRCS) $(RVV_SRCS_ALL) syscalls.cpp
    QEMU_ENV = 
endif

# ==========================================
# 4. Flags
# ==========================================
# RISC-V Flags for Vector Extension support (ضفنا -static لضمان الاستقرار)
RISCV_FLAGS = -march=rv64gcv -O2 -static

# Google Test Flags
GTEST_FLAGS = -lgtest -pthread

# ==========================================
# 5. Targets الأساسية
# ==========================================
all: host riscv

# Build for the local host machine (Scalar only)
host:
	$(CXX_HOST) $(MAIN_SRCS) -o my_program_host

# Build for the target architecture with RVV enabled (-DUSE_RVV=1)
riscv:
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=1 $(RISCV_SRCS) -o my_program_riscv

# Build the Google Test executable for Host
test:
	$(CXX_HOST) $(TEST_SRCS) $(GTEST_FLAGS) -o my_tests

# ==========================================
# 6. RVV Equivalence Test Targets (اختبارات التطابق بتاعتك)
# ==========================================
rvv_test:
	$(CXX_RISCV) $(RISCV_FLAGS) $(RVV_TEST_SRCS) -o test_rvv

# تشغيل اختبار التطابق على كذا VLEN ومراعاة البيئة
run_rvv: rvv_test
	@echo "--- Testing with VLEN=128 ---"
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=128 ./test_rvv
	@echo "--- Testing with VLEN=256 ---"
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=256 ./test_rvv
	@echo "--- Testing with VLEN=512 ---"
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./test_rvv

# ==========================================
# 7. الـ Commands السحرية بتاعتك (Run, Sweep, VLEN)
# ==========================================
# تشغيل البرنامج العادي على الـ QEMU
run: clean riscv
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./my_program_riscv
	python3 convert.py

# اختبار الأداء على VLENs مختلفة لملف الـ RISC-V الأساسي
run_vlen: clean riscv
	@echo "===== VLEN = 128 ====="
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=128 ./my_program_riscv
	@echo ""
	@echo "===== VLEN = 256 ====="
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=256 ./my_program_riscv
	@echo ""
	@echo "===== VLEN = 512 ====="
	qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./my_program_riscv

# الـ Optimization Sweep (تجارب الـ Compiler Flags مع الـ Scalar)
sweep:
	@echo "=== O0 ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O0 $(RISCV_SRCS) -o canny_O0
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O0

	@echo "=== O2 ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O2 $(RISCV_SRCS) -o canny_O2
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O2

	@echo "=== O3 ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -O3 $(RISCV_SRCS) -o canny_O3
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_O3

	@echo "=== Os ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -Os $(RISCV_SRCS) -o canny_Os
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_Os

	@echo "=== Ofast ==="
	$(CXX_RISCV) $(RISCV_FLAGS) -DUSE_RVV=0 -Ofast $(RISCV_SRCS) -o canny_Ofast
	/usr/bin/time -f "Elapsed: %e s" qemu-riscv64 $(QEMU_ENV) -cpu max,vlen=512 ./canny_Ofast

# تحويل الصور
png:
	python3 convert.py

# التنظيف المتكامل
clean:
	rm -f my_program_host my_program_riscv my_tests test_rvv canny_O0 canny_O2 canny_O3 canny_Os canny_Ofast
	rm -f output_*.raw output_*.png