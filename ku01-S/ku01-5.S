    .global delegate
delegate:
    push %ebp
    mov %esp, %ebp
    push %ebx
    mov 8(%ebp), %ebx
    mov 12(%ebp), %ecx
    lea 12(%ebp, %ebx, 4), %edx
cycle:
    test %ebx, %ebx
    jz compute
    push (%edx)
    sub $4, %edx
    dec %ebx
    jmp cycle
compute:
    call *%ecx
end:
    mov %ebp, %esp
    sub $4, %esp
    pop %ebx
    pop %ebp
    ret
