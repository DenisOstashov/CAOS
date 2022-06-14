#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void fprint7(FILE* f, int64_t number) {
    char num[25];
    int last = 0;
    if (number == INT64_MIN) {
        num[0] = '1';
        ++last;
        number /= 7;
    }
    if (number < 0) {
        fputc('-', f);
        number = -number;
    }
    while (number) {
        num[last] = (char)(number % 7);
        num[last] += '0';
        number /= 7;
        ++last;
    }
    for (int i = last - 1; i >= 0; --i) {
        fputc(num[i], f);
    }
    if (last == 0) {
        fputc('0', f);
    }
}
