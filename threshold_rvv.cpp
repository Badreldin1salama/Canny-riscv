#include "rvv_kernels.h"

void applyDoubleThreshold_RVV(const uint8_t* nmsImage,
                              uint8_t* thresholdResult,
                              int width, int height,
                              uint8_t lowThreshold, uint8_t highThreshold) {
    
    int size = width * height;
    thresholdResult;

    const uint8_t* in_ptr = nmsImage;
    uint8_t* out_ptr = thresholdResult;

    size_t vl; // Vector Length
    // Strip-mining loop
    for (; size > 0; size -= vl, in_ptr += vl, out_ptr += vl) {
        
        // 1. تحديد حجم المتجه (8-bit elements, LMUL=8 لزيادة كمية البكسلات المعالجة)
        vl = __riscv_vsetvl_e8m8(size);

        // 2. تحميل البكسلات من الذاكرة إلى مسجلات المتجهات (Vector Registers)
        vuint8m8_t v_nms = __riscv_vle8_v_u8m8(in_ptr, vl);

        // 3. مقارنات موازية (Vector-Scalar Comparisons)
        // هل البكسل >= highThreshold؟ (ينتج ماسك منطقي Boolean Mask)
        vbool1_t mask_high = __riscv_vmsgeu_vx_u8m8_b1(v_nms, highThreshold, vl);
        // هل البكسل >= lowThreshold؟
        vbool1_t mask_low  = __riscv_vmsgeu_vx_u8m8_b1(v_nms, lowThreshold, vl);

        // 4. بناء النتيجة
        // نبدأ بمتجه كله أصفار
        vuint8m8_t v_res = __riscv_vmv_v_x_u8m8(0, vl);
        
        // إذا كان يحقق Low، ضع 128
        v_res = __riscv_vmerge_vxm_u8m8(v_res, 128, mask_low, vl);
        
        // إذا كان يحقق High، استبدل القيمة بـ 255 (يغطي على الـ 128)
        v_res = __riscv_vmerge_vxm_u8m8(v_res, 255, mask_high, vl);

        // 5. حفظ النتيجة في الذاكرة
        __riscv_vse8_v_u8m8(out_ptr, v_res, vl);
    }
}

