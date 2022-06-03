#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

union ufloat {
    unsigned u;
    float f;
};

enum {
    GET_SGN = 31,
    DESTROY_SGN = 0x7fffffff,
    GET_EXP = 23,
    DESTROY_EXP = 0x007fffff
};

int main() {
    union ufloat uf;
    while (scanf("%f", &uf.f) == 1) {
        unsigned sign = uf.u >> GET_SGN;
        uf.u &= DESTROY_SGN;
        unsigned exp = (uf.u >> GET_EXP);
        unsigned man = uf.u & DESTROY_EXP;
        printf("%u %u %x\n", sign, exp, man);
    }
}