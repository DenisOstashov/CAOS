#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

union ufloat {
    uint32_t u;
    float f;
};

enum {
    GET_SGN = 31,
    DESTROY_SGN = 0x7fffffff,
    GET_EXP = 23,
    DESTROY_EXP = 0x007fffff,
    MAX_EXP = 255
};

FPClass fpclassf(float value, int *psign) {
    union ufloat uf;
    uf.f = value;
    unsigned sign = uf.u >> GET_SGN;
    uf.u &= DESTROY_SGN;
    if (sign == 0) {
        *psign = 0;
    } else {
        *psign = 1;
    }
    unsigned exp = (uf.u >> GET_EXP);
    unsigned man = uf.u & DESTROY_EXP;
    if (exp == 0 && man == 0) {
        return FFP_ZERO;
    }
    if (exp == MAX_EXP && man == 0) {
        return FFP_INF;
    }
    if (exp == MAX_EXP) {
        *psign = 0;
        return FFP_NAN;
    }
    if (exp == 0) {
        return FFP_DENORMALIZED;
    } else {
        return FFP_NORMALIZED;
    }
}