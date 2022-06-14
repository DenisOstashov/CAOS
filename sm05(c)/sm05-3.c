#include <stdint.h>

uint32_t satsum(uint32_t v1, uint32_t v2) {
    uint32_t ans = v1 + v2;
    if (ans >= v1 && ans >= v2) {
        return ans;
    } else {
        return -1;
    }
}
