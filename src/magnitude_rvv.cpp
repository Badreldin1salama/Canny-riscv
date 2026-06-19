#include "rvv_kernels.h"
#include <cstdlib>
#include <algorithm>

void computeMagnitude_RVV(const int16_t* Gx,
                          const int16_t* Gy,
                          uint8_t* mag,
                          int width, int height) {
    
    int size = width * height;

    // تم حجز مصفوفة مؤقتة بحجم الصورة لتخزين القيم الـ 32-bit
    int32_t* tempMag = new int32_t[size];

    const int16_t* gx_ptr = Gx;
    const int16_t* gy_ptr = Gy;
    int32_t* temp_ptr = tempMag;

    int32_t maxMag = 0;
    int remaining_size = size;

    // -------------------------------------------------------------
    // Pass 1: حساب L1 norm (|Gx| + |Gy|) وإيجاد القيمة العظمى maxMag
    // -------------------------------------------------------------
    size_t vl;
    // سنستخدم متجه ذو قيمة واحدة لعمل عملية الاختزال (Reduction) لإيجاد القيمة العظمى
    vint32m1_t v_max_res = __riscv_vmv_v_x_i32m1(0, 1); 

    for (; remaining_size > 0; remaining_size -= vl, gx_ptr += vl, gy_ptr += vl, temp_ptr += vl) {
        vl = __riscv_vsetvl_e16m4(remaining_size);

        // 1. تحميل Gx و Gy
        vint16m4_t v_gx = __riscv_vle16_v_i16m4(gx_ptr, vl);
        vint16m4_t v_gy = __riscv_vle16_v_i16m4(gy_ptr, vl);

        // 2. حساب القيمة المطلقة لـ Gx
        vbool4_t mask_gx_neg = __riscv_vmslt_vx_i16m4_b4(v_gx, 0, vl);
        vint16m4_t v_gx_neg = __riscv_vrsub_vx_i16m4(v_gx, 0, vl);
        vint16m4_t v_gx_abs = __riscv_vmerge_vvm_i16m4(v_gx, v_gx_neg, mask_gx_neg, vl);

        // 3. حساب القيمة المطلقة لـ Gy
        vbool4_t mask_gy_neg = __riscv_vmslt_vx_i16m4_b4(v_gy, 0, vl);
        vint16m4_t v_gy_neg = __riscv_vrsub_vx_i16m4(v_gy, 0, vl);
        vint16m4_t v_gy_abs = __riscv_vmerge_vvm_i16m4(v_gy, v_gy_neg, mask_gy_neg, vl);

        // 4. الجمع |Gx| + |Gy| بنوع 16-bit
        vint16m4_t v_sum16 = __riscv_vadd_vv_i16m4(v_gx_abs, v_gy_abs, vl);

        // 5. توسيع النتيجة إلى 32-bit لمنع الـ Overflow أثناء الضرب اللاحق وتخزينها في tempMag
        vint32m8_t v_sum32 = __riscv_vsext_vf2_i32m8(v_sum16, vl);
        __riscv_vse32_v_i32m8(temp_ptr, v_sum32, vl);

        // 6. تحديث القيمة العظمى المحلية في هذا الجزء باستخدام Reduction
        v_max_res = __riscv_vredmax_vs_i32m8_i32m1(v_sum32, v_max_res, vl);
    }

    // استخراج الـ maxMag النهائي من بؤرة المتجه
    maxMag = __riscv_vmv_x_s_i32m1_i32(v_max_res);
    if (maxMag == 0) maxMag = 1; // منع القسمة على صفر

    // -------------------------------------------------------------
    // Pass 2: التطبيع إلى النطاق [0, 255] والتخزين في مصفوفة mag النهائية
    // -------------------------------------------------------------
    remaining_size = size;
    const int32_t* temp_read_ptr = tempMag;
    uint8_t* out_ptr = mag;

    for (; remaining_size > 0; remaining_size -= vl, temp_read_ptr += vl, out_ptr += vl) {
        vl = __riscv_vsetvl_e32m8(remaining_size);

        // 1. تحميل قيمة الـ 32-bit المؤقتة
        vint32m8_t v_temp = __riscv_vle32_v_i32m8(temp_read_ptr, vl);

        // 2. ضرب القيمة في 255
        vint32m8_t v_prod = __riscv_vmul_vx_i32m8(v_temp, 255, vl);

        // 3. القسمة على maxMag
        vint32m8_t v_norm = __riscv_vdiv_vx_i32m8(v_prod, maxMag, vl);

        // 4. ضغط النتيجة (Narrowing) من 32-bit إلى 16-bit ثم إلى 8-bit
        vint16m4_t v_norm16 = __riscv_vncvt_x_x_w_i16m4(v_norm, vl);
        
        size_t vl_e8 = __riscv_vsetvl_e8m2(remaining_size);
        vuint8m2_t v_out = __riscv_vncvt_x_x_w_u8m2(__riscv_vreinterpret_v_i16m4_u16m4(v_norm16), vl_e8);

        // 5. تخزين النتيجة النهائية
        __riscv_vse8_v_u8m2(out_ptr, v_out, vl_e8);
    }

    // 7. تحرير الذاكرة المؤقتة لمنع الـ Memory Leak
    delete[] tempMag;
}