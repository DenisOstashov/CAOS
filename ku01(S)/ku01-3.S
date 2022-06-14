#include "sys/syscall.h"
    .text
    .equ stdin, 0
    .equ stdout, 1
    .global mul2
mul2:
    push %ebp
    mov %esp, %ebp
    push %ebx
    mov 8(%ebp), %ebx
    mov (%ebx), %eax
    mov $2, %ecx
    imull %ecx
    jo end
    mov %eax, (%ebx)
end:
    pop %ebx
    mov %esp, %ebp
    pop %ebp
    xor %eax, %eax
    ret
