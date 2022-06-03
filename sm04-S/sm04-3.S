#include "sys/syscall.h"
    .text
    .global обработка
обработка:
    push %ebp
    mov %esp, %ebp
    push %ebx
    push %esi
    push %edi
    xor %ecx, %ecx
cycle:
    test %ebx, %ebx
    jz end
    mov 4(%ebx, %esi, 4), %eax
    add 52(%ebx, %edi, 4), %eax
    cdq
    push %edi
    mov $2, %edi
    idiv %edi
    pop %edi
    add %eax, %ecx
    mov (%ebx), %ebx
    jmp cycle
end:
    pop %edi
    pop %esi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    push (%esp)
    mov %ecx, 4(%esp)
    ret
