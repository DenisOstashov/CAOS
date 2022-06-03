    .text
    .global myhypot
myhypot:
    push %ebp
    mov %esp, %ebp
    movsd 8(%ebp), %xmm1
    movsd 16(%ebp), %xmm2
    mulsd %xmm1, %xmm1
    mulsd %xmm2, %xmm2
    addsd %xmm2, %xmm1
    sqrtsd %xmm1, %xmm1
    movl 24(%ebp), %ecx
    movsd %xmm1, (%ecx)
    mov %ebp, %esp
    pop %ebp
    ret
