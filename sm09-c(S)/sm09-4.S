    .global myexp
myexp:
    push %ebp
    mov %esp, %ebp
    movsd 8(%ebp), %xmm1
    movsd %xmm1, %xmm2
    movsd one, %xmm3
    movsd %xmm3, %xmm4
loop:
    movsd %xmm3, %xmm5
    addsd %xmm2, %xmm3
    mulsd %xmm1, %xmm2
    addsd one, %xmm4
    divsd %xmm4, %xmm2
    comisd %xmm3, %xmm5
    jne loop
    movl 16(%ebp), %ecx
    movsd %xmm3, (%ecx)
    mov %ebp, %esp
    pop %ebp
    ret

    .data
one: .double 1
