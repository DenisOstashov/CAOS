#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

int main() {
    int family_id, model, ecx, edx, extended_family_id, extended_model_id;
    asm volatile (
        "movl $1, %%eax\t\n"
        "cpuid\t\n"
        "movl %%ecx, %2\t\n"
        "movl %%edx, %3\t\n"
        "shr $4, %%eax\t\n"
        "movl %%eax, %1\t\n"
        "andl $0x0000000f, %1\t\n"
        "shr $4, %%eax\t\n"
        "movl %%eax, %0\t\n"
        "andl $0x0000000f, %0\t\n"
        "shr $8, %%eax\t\n"
        "movl %%eax, %4\t\n"
        "andl $0x0000000f, %4\t\n"
        "shr $4, %%eax\t\n"
        "movl %%eax, %5\t\n"
        "andl $0x000000ff, %5\t\n"
        : "=m" (family_id), "=m" (model), "=m" (ecx), "=m" (edx),
            "=m" (extended_model_id), "=m" (extended_family_id)
        :
        : "%eax", "%ecx", "%edx", "%ebx");
    int real_family, real_model;
    if (family_id == 6 || family_id == 15) {
        real_model = (extended_model_id << 4) + model;
    } else {
        real_model = model;
    }
    if (family_id == 15) {
        real_family = extended_family_id + family_id;
    } else {
        real_family = family_id;
    }
    printf("family=%d model=%d ecx=0x%x edx=0x%x\n",
           real_family, real_model, ecx, edx);
}