    .text
    .global discrim
discrim:
    push %ebp
    mov %esp, %ebp
    movss 8(%ebp), %xmm1
    movss 12(%ebp), %xmm2
    movss 16(%ebp), %xmm3
    mulss %xmm1, %xmm3
    mulss %xmm2, %xmm2
    mulss four, %xmm3
    subss %xmm3, %xmm2
    movl 20(%ebp), %ecx
    movss %xmm2, (%ecx)
    mov %ebp, %esp
    pop %ebp
    ret

    .data
four: .float 4
