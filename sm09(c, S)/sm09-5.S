    .global dot_product
dot_product:
    push %ebp
    mov %esp, %ebp
    push %ebx
    mov 8(%ebp), %eax
    mov 12(%ebp), %ecx
    mov 16(%ebp), %edx
    xorps %xmm3, %xmm3
    xor %ebx, %ebx
fast:
    cmp $4, %eax
    jl sum
    movups (%ecx, %ebx, 4), %xmm1
    movups (%edx, %ebx, 4), %xmm2
    sub $4, %eax
    add $4, %ebx
    mulps %xmm2, %xmm1
    addps %xmm1, %xmm3
    jmp fast
sum:
    haddps %xmm3, %xmm3
    haddps %xmm3, %xmm3
slow:
    test %eax, %eax
    jz exit
    movss (%ecx, %ebx, 4), %xmm1
    movss (%edx, %ebx, 4), %xmm2
    dec %eax
    inc %ebx
    mulss %xmm2, %xmm1
    addss %xmm1, %xmm3
    jmp slow
exit:
    mov 20(%ebp), %eax
    movss %xmm3, (%eax)
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret
