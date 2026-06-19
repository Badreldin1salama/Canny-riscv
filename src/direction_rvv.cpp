#include "rvv_kernels.h"

void computeDirection_RVV(const int16_t* Gx, const int16_t* Gy, uint8_t* direction, int width, int height)
{
    int size = width * height;
    direction;

    const int16_t* gx_ptr = Gx;
    const int16_t* gy_ptr = Gy;
    uint8_t* out_ptr = direction;

    size_t vl;

    for (; size > 0; size -= vl,
                         gx_ptr += vl,
                         gy_ptr += vl,
                         out_ptr += vl)
    {
        vl = __riscv_vsetvl_e16m4(size);

        vint16m4_t v_gx = __riscv_vle16_v_i16m4(gx_ptr, vl);
        vint16m4_t v_gy = __riscv_vle16_v_i16m4(gy_ptr, vl);

        vbool4_t gx_neg = __riscv_vmslt_vx_i16m4_b4(v_gx, 0, vl);
        vbool4_t gy_neg = __riscv_vmslt_vx_i16m4_b4(v_gy, 0, vl);

        vint16m4_t gx_abs =
            __riscv_vmerge_vvm_i16m4(
                v_gx,
                __riscv_vrsub_vx_i16m4(v_gx, 0, vl),
                gx_neg,
                vl);

        vint16m4_t gy_abs =
            __riscv_vmerge_vvm_i16m4(
                v_gy,
                __riscv_vrsub_vx_i16m4(v_gy, 0, vl),
                gy_neg,
                vl);
        // ay*5
        vint16m4_t ay5 =
            __riscv_vmul_vx_i16m4(gy_abs, 5, vl);

        // ax*2
        vint16m4_t ax2 =
            __riscv_vmul_vx_i16m4(gx_abs, 2, vl);

        // ax*12
        vint16m4_t ax12 =
            __riscv_vmul_vx_i16m4(gx_abs, 12, vl);

        // ay*5 < ax*2
        vbool4_t mask_horizontal =
            __riscv_vmslt_vv_i16m4_b4(ay5, ax2, vl);

        // ay*5 > ax*12
        vbool4_t mask_vertical =
            __riscv_vmsgt_vv_i16m4_b4(ay5, ax12, vl);

        // output = 45
        vuint8m2_t result =
            __riscv_vmv_v_x_u8m2(45, vl);
           
    
        // gx and gy have same sign
        vbool4_t gx_pos =
            __riscv_vmsgt_vx_i16m4_b4(v_gx, 0, vl);

        vbool4_t gy_pos =
            __riscv_vmsgt_vx_i16m4_b4(v_gy, 0, vl);

        vbool4_t both_pos =
            __riscv_vmand_mm_b4(gx_pos, gy_pos, vl);

        vbool4_t both_neg =
            __riscv_vmand_mm_b4(gx_neg, gy_neg, vl);

        vbool4_t same_sign =
            __riscv_vmor_mm_b4(both_pos, both_neg, vl);

        // diagonal 135
        result =
            __riscv_vmerge_vxm_u8m2(
                result,
                135,
                same_sign,
                vl);

        // vertical
        result =
            __riscv_vmerge_vxm_u8m2(
                result,
                90,
                mask_vertical,
                vl);

        // horizontal
        result =
            __riscv_vmerge_vxm_u8m2(
                result,
                0,
                mask_horizontal,
                vl);

        __riscv_vse8_v_u8m2(out_ptr, result, vl);
            }
}