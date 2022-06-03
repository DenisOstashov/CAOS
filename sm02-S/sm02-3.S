    .text
    .global process
process:
    push %ebp
    mov %esp, %ebp
    push %ebx
    xor %ebx, %ebx
read:
    call readi32
    jc write
    push %eax
    inc %ebx
    jmp read
write:
    test %ebx, %ebx
    jz exit
    dec %ebx
    pop %eax
    call writei32
    jmp write
exit:
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret
