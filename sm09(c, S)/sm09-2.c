#include <sys/syscall.h>

void _start() {
    const char *str = "hello world\n";
    asm (
    "movl $4, %%eax\n\t"
    "movl $1, %%ebx\n\t"
    "movl %0, %%ecx\n\t"
    "movl $12, %%edx\n\t"
    "int $0x80\n\t"
    :
    : "r" (str)
    : "%eax", "%ebx", "%ecx", "%edx");
    asm volatile (
    "movl $1, %%eax\n\t"
    "xorl %%ebx, %%ebx\n\t"
    "int $0x80\n\t"
    :
    :
    : "%eax", "%ebx");
}