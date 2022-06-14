#include "sys/syscall.h"
    .text
    .global alloc_mem
alloc_mem:
    push %ebp
    mov %esp, %ebp
    push %ebx
    push %esi
    push %edi
    mov 8(%ebp), %edi
    lea freelist, %edx
search:
    mov (%edx), %esi
    test %esi, %esi
    jz not_found
    movl (%esi), %ebx
    cmp %edi, %ebx
    jge found
    lea 4(%esi), %edx
    jmp search
found:
    mov 4(%esi), %ebx
    mov %ebx, (%edx)
    lea 4(%esi), %eax
    jmp end
not_found:
    mov heapend, %eax
    test %eax, %eax
    jnz good_heapend
    mov $SYS_brk, %eax
    mov $0x0, %ebx
    int $0x80
    mov %eax, heapend
good_heapend:
    test %edi, %edi
    jnz start_align
    mov $0x4, %edi
    jmp aligned
start_align:
    xor %edx, %edx
    mov %edi, %eax
    mov $0x4, %ecx
    div %ecx
    test %edx, %edx
    jz aligned
    sub %edx, %ecx
    add %ecx, %edi
    jc zero
aligned:
    mov $SYS_brk, %eax
    mov heapend, %ebx
    add %edi, %ebx
    jc zero
    add $0x4, %ebx
    jc zero
    int $0x80
    cmp %eax, %ebx
    jne zero
    mov heapend, %esi
    mov %eax, heapend
    movl %edi, (%esi)
    lea 4(%esi), %eax
    jmp end
zero:
    xor %eax, %eax
end:
    pop %edi
    pop %esi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret


    .data
heapend: .int 0	
