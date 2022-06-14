     .text
     .global partition
partition:
    push %ebp
    mov %esp, %ebp
    mov %ebx, %ecx
    push %ebx
    xor %ebx, %ebx
    xor %edx, %edx
cycle:
    movw (%ecx, %edx, 2), %ax
    cmp %ax, %di
    jle found
    inc %edx
    jmp cycle
found:
    mov %edx, %ebx
    inc %ebx
find:
    cmp %esi, %ebx
    je exit
    movw (%ecx, %ebx, 2), %ax
    cmp %ax, %di
    jg swap
    inc %ebx
    jmp find
swap:
    push %esi    
    xor %esi, %esi
    movw (%ecx, %edx, 2), %si
    movw %ax, (%ecx, %edx, 2)
    movw %si, (%ecx, %ebx, 2)
    pop %esi
    inc %edx
    inc %ebx
    jmp find
exit:
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret
