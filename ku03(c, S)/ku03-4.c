#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

enum {
    GET_SGN = 15,
    DESTROY_SGN = 0b0111111111111111,
    GET_EXP = 10,
    DESTROY_EXP = 0b0000001111111111,
    MAX_EXP = 0b11111,
    REPRESENTABLE = 0b11100,
    MAX_REPRESENTABLE = 0b0111101111111111,
    FIRST_DENORMALISED = 0b1000000000,
    SECOND_DENORMALISED = 0b0100000000
};


uint16_t sat_mul4_half(uint16_t f) {
    uint16_t copy = f;
    uint16_t sign = f >> GET_SGN;
    f &= DESTROY_SGN;
    uint16_t exp = f >> GET_EXP;
    uint16_t man = f & DESTROY_EXP;
    if ((exp == 0 && man == 0) || exp == MAX_EXP) {
        return copy;
    }
    if (exp > REPRESENTABLE) {
        if (sign == 1) {
            return MAX_REPRESENTABLE | (1 << GET_SGN);
        } else {
            return MAX_REPRESENTABLE;
        }
    }
    if (exp == 0) {
        if (man & FIRST_DENORMALISED) {
            exp = 2;
            man &= ~FIRST_DENORMALISED;
            man <<= 1;
        } else if (man & SECOND_DENORMALISED) {
            exp = 1;
            man &= ~SECOND_DENORMALISED;
            man <<= 2;
        } else {
            man <<= 2;
        }
    } else {
        exp += 2;
    }
    return (sign << GET_SGN) | (exp << GET_EXP) | man;
}