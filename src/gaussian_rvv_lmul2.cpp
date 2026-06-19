#include "gaussian_rvv.h"
#include "gaussian.h"
#include <riscv_vector.h>

void applyGaussianBlur_RVV_LMUL2(
    const uint8_t* inputImage,
    uint8_t* outputImage,
    int width,
    int height)
{
    outputImage;

const int paddedWidth  = width + 4;
const int paddedHeight = height + 4;

std::vector<uint8_t> padded(
    paddedWidth * paddedHeight,
    0);

for (int row = 0; row < height; ++row)
{
    for (int col = 0; col < width; ++col)
    {
        padded[(row + 2) * paddedWidth + (col + 2)] =
            inputImage[row * width + col];
    }
}

    const int16_t kernel[5][5] =
    {
        {1, 4, 7, 4, 1},
        {4,16,26,16,4},
        {7,26,41,26,7},
        {4,16,26,16,4},
        {1, 4, 7, 4, 1}
    };

    const int kernelSum = 273;

    for (int row = 0; row < height; ++row)
    {
    int col = 0;

        while (col < width)
        {
 size_t vl =
    __riscv_vsetvl_e8mf2(
        width - col);

vint32m2_t sum =
    __riscv_vmv_v_x_i32m2(
        0,
        vl);

            for (int ky = -2; ky <= 2; ++ky)
            {
                for (int kx = -2; kx <= 2; ++kx)
                {
const uint8_t* src =
    &padded[
        (row + ky + 2) * paddedWidth +
        (col + kx + 2)
    ];
    vuint8mf2_t pixels =
    __riscv_vle8_v_u8mf2(
        src,
        vl);
        vuint16m1_t pixels_u16 = 
    __riscv_vwcvtu_x_x_v_u16m1(
        pixels,
        vl);

vint16m1_t pixels_i16 =
    __riscv_vreinterpret_v_u16m1_i16m1(
        pixels_u16);

sum =
    __riscv_vwmacc_vx_i32m2(
        sum,
        kernel[ky + 2][kx + 2],
        pixels_i16,
        vl);
                    
                }
            }

            vint32m2_t result32 =
    __riscv_vdiv_vx_i32m2(
        sum,
        kernelSum,
        vl);

vint16m1_t result16 =
    __riscv_vncvt_x_x_w_i16m1(
        result32,
        vl);

vuint16m1_t result16_u =
    __riscv_vreinterpret_v_i16m1_u16m1(
        result16);

vuint8mf2_t result8 =
    __riscv_vncvt_x_x_w_u8mf2(
        result16_u,
        vl);


__riscv_vse8_v_u8mf2(
    &outputImage[row * width + col],
    result8,
    vl);
/*            int32_t value = sum / kernelSum;

            if (value < 0) value = 0;
            if (value > 255) value = 255;

            outputImage[row * width + col] =
                static_cast<uint8_t>(value); */
                 
                col += vl;       
                
        }

    }
}