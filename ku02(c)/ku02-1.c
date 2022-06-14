#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void tinyconv(uint8_t bytes[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        uint8_t fir = bytes[i] << 7;
        uint8_t sec = (bytes[i] & 2) << 5;
        uint8_t thi = (bytes[i] & 4) << 3;
        uint8_t fou = (bytes[i] & 8) << 1;
        uint8_t fif = (bytes[i] & 16) >> 1;
        uint8_t six = (bytes[i] & 32) >> 3;
        uint8_t sev = (bytes[i] & 64) >> 5;
        uint8_t eig = bytes[i] >> 7;
        bytes[i] = fir | sec | thi | fou | fif | six | sev | eig;
    }
}