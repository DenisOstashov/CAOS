    .text
    .global list_reverse
list_reverse:
    push %ebp
    mov %esp, %ebp
    push %ebx
    xor %ecx, %ecx
    mov head, %eax
    test %eax, %eax
    jz exit
cycle:
    mov 8(%eax), %ebx
    mov %ecx, 8(%eax)
    test %ebx, %ebx
    jz exit
    mov %eax, %ecx
    mov %ebx, %eax
    jmp cycle
exit:
    mov %eax, head
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret
