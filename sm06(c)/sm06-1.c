#include <stdio.h>
#include <inttypes.h>

int main() {
    uint32_t first, second, result;
    scanf("%" SCNu32 "%" SCNu32, &first, &second);
    result = (first / 2) + (second / 2) + (first & second & 1);
    printf("%" PRIu32 "\n", result);
}
