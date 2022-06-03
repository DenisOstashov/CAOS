#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int convertable(uint32_t value) {
    uint32_t interval = 0x01000000;//2^24
    uint32_t mod = 1;
    while (value > interval && interval < 0x80000000) {
        mod *= 2;
        interval *= 2;
    }
    if (value > interval) {
        mod *= 2;
    }
    return (value % mod == 0);
}

int main() {
    uint32_t value;
    while (scanf("%" SCNu32, &value) == 1) {
        printf("%d\n", convertable(value));
    }
}