#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

union ufloat {
    uint32_t u;
    float f;
};

enum {
    GET_SGN_F32 = 31,
    DESTROY_SGN_F32 = 0x7fffffff,
    GET_EXP_F32 = 23,
    DESTROY_EXP_F32 = 0x007fffff,
    MAX_EXP_F32 = 255,
    BIAS_F32 = 127,
    GET_SGN_F16 = 15,
    GET_EXP_F16 = 10,
    MAN_OVERFLOW_F16 = 0b10000000000,
    MAX_EXP_F16 = 31,
    BIAS_F16 = 15,
    NAN_F16_MAN = 1,
    GET_MAN_REMAINS = 0b1111111111111,
    SIGNIFICANT_BIT = 0b1000000000000,
};

uint16_t float_to_fp16(float f) {
    union ufloat uf;
    uf.f = f;
    uint32_t sign_f32 = uf.u >> GET_SGN_F32;
    uf.u &= DESTROY_SGN_F32;
    int32_t exp_f32 = (int32_t)(uf.u >> GET_EXP_F32);
    uint32_t man_f32 = uf.u & DESTROY_EXP_F32;
    uint16_t sign_f16 = sign_f32;
    int32_t exp_f16 = exp_f32 - BIAS_F32 + BIAS_F16;
    uint16_t man_f16;
    if (exp_f32 == MAX_EXP_F32) {
        if (man_f32 == 0) {
            return (sign_f16 << GET_SGN_F16) | (MAX_EXP_F16 << GET_EXP_F16);
        } else {
            return (sign_f16 << GET_SGN_F16) | (MAX_EXP_F16 << GET_EXP_F16) | NAN_F16_MAN;
        }
    }
    man_f16 = man_f32 >> 13;
    man_f32 &= GET_MAN_REMAINS;
    int32_t bit = SIGNIFICANT_BIT;
    if (bit & man_f32) {
        bit >>= 1;
        while (bit > 0 && !(bit & man_f32)) {
            bit >>= 1;
        }
        if (bit || (man_f16 & 1)) {
            man_f16 += 1;
            if (man_f16 == MAN_OVERFLOW_F16) {
                man_f16 = 0;
                exp_f16 += 1;
            }
        }
    }
    if (exp_f16 <= 0 || exp_f16 >= MAX_EXP_F16) {
        man_f16 = 0;
        exp_f16 = MAX_EXP_F16;
    }
    return (sign_f16 << GET_SGN_F16) | (exp_f16 << GET_EXP_F16) | man_f16;
}