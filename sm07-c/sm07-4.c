#include <stdint.h>

int imull_overflow(int64_t a, int64_t b, int64_t *res) {
    *res = (int64_t)(((uint64_t)a) * ((uint64_t)b));
    if (a == 0 || b == 0) {
        return 0;
    }
    uint64_t unsigned_max = -1;
    int64_t signed_max = (int64_t) (unsigned_max >> 1);
    int64_t signed_min = (-signed_max) - 1;
    if (a > 0 && b > 0) {
        int64_t max_good = signed_max / a;
        if (b > max_good) {
            return 1;
        }
    } else if (a < 0 && b < 0) {
        int64_t min_good = signed_max / a;
        if (b < min_good) {
            return 1;
        }
    } else if (a > 0) {
        int64_t min_good = signed_min / a;
        if (b < min_good) {
            return 1;
        }
    } else {
        int64_t min_good = signed_min / b;
        if (a < min_good) {
            return 1;
        }
    }
    return 0;
}
