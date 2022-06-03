#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

union ufloat {
    uint32_t u;
    float f;
};

enum {
    GET_EXP = 23,
    DESTROY_EXP = 0x007fffff,
    BIAS = 127,
};

void myfloorf(float *arg) {
    union ufloat uf;
    uf.f = *arg;
    uint32_t exp = (uf.u >> GET_EXP);
    uint32_t man = uf.u & DESTROY_EXP;
    int32_t deg = (int32_t) exp - BIAS;
    if (deg < 0) {
        *arg = 0;
        return;
    } else if (deg >= GET_EXP) {
        return;
    }
    uint32_t offset = GET_EXP - deg;
    man >>= offset;
    man <<= offset;
    uf.u = (exp << GET_EXP) | man;
    *arg = uf.f;
}