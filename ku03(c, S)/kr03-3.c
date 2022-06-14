#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

void print_parent_arg() {
    int32_t value;

      asm volatile (
    "mov %%ebp, %%eax\n\t"
       "mov (%%eax), %%eax\n\t"
    "movl 8(%%eax), %%eax\n\t"
       "mov %%eax, %0\n\t  "
    : "=m" (value)
        :
    : "%eax");
    printf("%" PRIx32 "\n", value);
}
