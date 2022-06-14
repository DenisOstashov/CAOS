    .data
    .global A, B, R
A: .int 0
B: .int 0
R: .int 0
    .text
    .global process
process:
    push %ebp
    mov %esp, %ebp
    push %ebx
    mov A, %ebx
    mov B, %ecx
    xor %eax, %eax
cycle:
    test $0x1, %ebx
    jz shift
    add %ecx, %eax
shift:
    shl $0x1, %ecx
    shr $0x1, %ebx
    jnz cycle
    mov %eax, R
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret
