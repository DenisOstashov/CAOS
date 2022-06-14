#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

uint64_t comp() {
    uint32_t a, d;
    asm volatile (
    "rdtsc\n\t"
    "mov %%eax, %0\n\t"
    "mov %%edx, %1\n\t"
    : "=m" (a), "=m"(d)
    :
    : "%eax", "%ecx", "%edx", "%ebx");
    uint64_t res = a;
    res <<= 32;
    res += d;
    return res;
}

void work_until_deadline(const uint64_t *deadline, void (*do_work)()) {
    while (comp() < *deadline) {
        do_work();
    }
}
