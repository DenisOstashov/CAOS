#include "inttypes.h"
#include "stdlib.h"
#include "stdio.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        return 1;
    }
    enum {
        BASE = 9,
    };
    int32_t val1 = strtol(argv[1], NULL, BASE);
    int32_t val2 = strtol(argv[2], NULL, BASE);
    int32_t res;
    printf("%d\n%d\n", __builtin_add_overflow(val1, val2, &res),
           !val2 || (val2 == -1 && val1 == INT32_MIN));
}