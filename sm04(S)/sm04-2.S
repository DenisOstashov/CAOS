#include "sys/syscall.h"
    .text
    .global free_mem
free_mem:
    push %ebp
    mov %esp, %ebp
    push %ebx
    push %esi
    push %edi
    mov 8(%ebp), %edi
    mov freelist, %esi
    sub $4, %edi
    mov %edi, freelist
    mov %esi, 4(%edi)
end:
    xor %eax, %eax
    pop %edi
    pop %esi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret

    .data
    .global freelist
freelist: .int 0
