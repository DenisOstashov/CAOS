    .text
    .global sort_by_abs
sort_by_abs:
    push %ebp
    mov %esp, %ebp
    push %ebx
    push %esi
    push %edi
    push $comp_by_abs
    pushl $2
    pushl 12(%ebp)
    pushl 8(%ebp)
    call qsort
    add $16, %esp
end:
    pop %edi
    pop %esi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret


comp_by_abs:
    push %ebp
    mov %esp, %ebp
    mov 8(%esp), %eax
    movw (%eax), %cx
    mov 12(%esp), %eax
    movw (%eax), %dx
    cmpw $0x8000, %cx
    jne first_not_min
    cmpw $0x8000, %dx
    je equal
    jmp greater
first_not_min:
    cmpw $0x8000, %dx
    je less
    testw %cx, %cx
    jge first_positive
    negw %cx
first_positive:
    testw %dx, %dx
    jge second_positive
    negw %dx
second_positive:
    cmp %dx, %cx
    jl less
    jg greater
equal:
    xor %eax, %eax
    jmp comp_end
greater:
    mov $1, %eax
    jmp comp_end
less:
    mov $-1, %eax
comp_end:
    mov %ebp, %esp
    pop %ebp
    ret
