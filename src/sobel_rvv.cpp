#include "sobel.h"
#include <riscv_vector.h>

void applySobel_RVV(
    const uint8_t* inputImage,
    int16_t* Gx,
    int16_t* Gy,
    int width,
    int height)
{
    int size = width * height;

    Gx;
    Gy;

    for (int row = 1; row < height - 1; ++row)
    {
        int col = 1;

        while (col < width - 1)
        {
            size_t vl =
                __riscv_vsetvl_e8m1(
                    (width - 1) - col);

const uint8_t* row0 =
    &inputImage[(row - 1) * width];

const uint8_t* row1 =
    &inputImage[row * width];

const uint8_t* row2 =
    &inputImage[(row + 1) * width];

vuint8m1_t p00 =
    __riscv_vle8_v_u8m1(
        row0 + col - 1,
        vl);

vuint8m1_t p01 =
    __riscv_vle8_v_u8m1(
        row0 + col,
        vl);

vuint8m1_t p02 =
    __riscv_vle8_v_u8m1(
        row0 + col + 1,
        vl);

vuint8m1_t p10 =
    __riscv_vle8_v_u8m1(
        row1 + col - 1,
        vl);

vuint8m1_t p12 =
    __riscv_vle8_v_u8m1(
        row1 + col + 1,
        vl);

vuint8m1_t p20 =
    __riscv_vle8_v_u8m1(
        row2 + col - 1,
        vl);

vuint8m1_t p21 =
    __riscv_vle8_v_u8m1(
        row2 + col,
        vl);

vuint8m1_t p22 =
    __riscv_vle8_v_u8m1(
        row2 + col + 1,
        vl);

vuint16m2_t p00_u16 = __riscv_vwcvtu_x_x_v_u16m2(p00, vl);
vuint16m2_t p02_u16 = __riscv_vwcvtu_x_x_v_u16m2(p02, vl);

vuint16m2_t p10_u16 = __riscv_vwcvtu_x_x_v_u16m2(p10, vl);
vuint16m2_t p12_u16 = __riscv_vwcvtu_x_x_v_u16m2(p12, vl);

vuint16m2_t p20_u16 = __riscv_vwcvtu_x_x_v_u16m2(p20, vl);
vuint16m2_t p22_u16 = __riscv_vwcvtu_x_x_v_u16m2(p22, vl);

vint16m2_t p00_s =
    __riscv_vreinterpret_v_u16m2_i16m2(p00_u16);

vint16m2_t p02_s =
    __riscv_vreinterpret_v_u16m2_i16m2(p02_u16);

vint16m2_t p10_s =
    __riscv_vreinterpret_v_u16m2_i16m2(p10_u16);

vint16m2_t p12_s =
    __riscv_vreinterpret_v_u16m2_i16m2(p12_u16);

vint16m2_t p20_s =
    __riscv_vreinterpret_v_u16m2_i16m2(p20_u16);

vint16m2_t p22_s =
    __riscv_vreinterpret_v_u16m2_i16m2(p22_u16);

vint16m2_t gx =
    __riscv_vsub_vv_i16m2(
        p02_s,
        p00_s,
        vl);

gx =
    __riscv_vadd_vv_i16m2(
        gx,
        p22_s,
        vl);

gx =
    __riscv_vsub_vv_i16m2(
        gx,
        p20_s,
        vl);

vint16m2_t p10_x2 =
    __riscv_vmul_vx_i16m2(
        p10_s,
        2,
        vl);

vint16m2_t p12_x2 =
    __riscv_vmul_vx_i16m2(
        p12_s,
        2,
        vl);

gx =
    __riscv_vsub_vv_i16m2(
        gx,
        p10_x2,
        vl);

gx =
    __riscv_vadd_vv_i16m2(
        gx,
        p12_x2,
        vl);

vuint16m2_t p01_u16 =
    __riscv_vwcvtu_x_x_v_u16m2(p01, vl);

vuint16m2_t p21_u16 =
    __riscv_vwcvtu_x_x_v_u16m2(p21, vl);

vint16m2_t p01_s =
    __riscv_vreinterpret_v_u16m2_i16m2(p01_u16);

vint16m2_t p21_s =
    __riscv_vreinterpret_v_u16m2_i16m2(p21_u16);

vint16m2_t gy =
    __riscv_vsub_vv_i16m2(
        p20_s,
        p00_s,
        vl);

gy =
    __riscv_vadd_vv_i16m2(
        gy,
        p22_s,
        vl);

gy =
    __riscv_vsub_vv_i16m2(
        gy,
        p02_s,
        vl);

vint16m2_t p01_x2 =
    __riscv_vmul_vx_i16m2(
        p01_s,
        2,
        vl);

vint16m2_t p21_x2 =
    __riscv_vmul_vx_i16m2(
        p21_s,
        2,
        vl);

gy =
    __riscv_vsub_vv_i16m2(
        gy,
        p01_x2,
        vl);

gy =
    __riscv_vadd_vv_i16m2(
        gy,
        p21_x2,
        vl);

int index = row * width + col;
__riscv_vse16_v_i16m2(
    &Gx[index],
    gx,
    vl);

__riscv_vse16_v_i16m2(
    &Gy[index],
    gy,
    vl);

            col += vl;
        }
    }
}