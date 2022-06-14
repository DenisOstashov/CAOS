#include <stdint.h>

int32_t satsum(int32_t v1, int32_t v2) {
    uint32_t unsigned_max = (-1);
    int32_t signed_max = (int32_t) (unsigned_max >> 1);
    int32_t signed_min = (-signed_max) - 1;
    if (v1 > 0 && v2 > 0 && signed_max - v1 < v2) {
        return signed_max;
    } else if (v1 < 0 && v2 < 0 && signed_min - v1 > v2) {
        return signed_min;
    }
    return v1 + v2;
}
