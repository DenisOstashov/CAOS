#include "sys/syscall.h"
    .text
    .equ stdin, 0
    .equ stdout, 1
    .global aggregate
aggregate:
    push %ebp
    mov %esp, %ebp
    push %ebx
    push %esi
    push %edi
    mov 8(%ebp), %esi
    mov 12(%ebp), %edi
    test %esi, %esi
    jnz continue
    movl $0, (%edi)
    movl $0, 4(%edi)
    movl $0, 8(%edi)
    jmp end
continue:
    movl $0x7fffffff, (%edi)
    movl $0x80000000, 4(%edi)
    movl $0, 8(%edi)
    xor %ecx, %ecx
cycle:
    test %esi, %esi
    jz compute
    inc %ecx
    mov (%esi), %eax
    cmp (%edi), %eax
    jg more
    movl %eax, (%edi)
more:
    cmp 4(%edi), %eax
    jl after
    movl %eax, 4(%edi)
after:
    cdq
    add %eax, sum
    adc %edx, sum+4
    movl 4(%esi), %esi
    jmp cycle
compute:
    mov sum, %eax
    mov sum+4, %edx
    idiv %ecx
    mov %eax, 8(%edi)
end:
    pop %edi
    pop %esi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret

    .data
sum: .int 0
     .int 0
