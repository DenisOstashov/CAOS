    .text
    .global strtou256
strtou256:
    push %ebp
    mov %esp, %ebp
    push %ebx
    push %edi
    push %esi
    push $0x0
    mov 8(%ebp), %edi
    mov 12(%ebp), %ebx
    mov 16(%ebp), %esi
    mov $0x8, %ecx
clear:
    movl $0x0, -4(%esi, %ecx, 4)
    dec %ecx
    jnz clear
cycle:
    cmpl %ebx, (%esp)
    je end
    xor %edx, %edx
    movb (%edi), %dl
    inc %edi
    cmpb $'9', %dl
    jg end
    cmpb $'0', %dl
    jl end
    subb $'0', %dl
    incl (%esp)
    push %edx
    call u256mul10reg
    pop %edx
    addl %edx, (%esi)
    jnc cycle
    mov %esi, %eax
    mov $0x7, %edx
adding:
    //incb (%eax) - по gdb понял, что ассемблер издевается:(
    addl $0x1, 4(%eax)
    jnc cycle
    add $0x4, %eax
    dec %edx
    jz cycle
    jmp adding
end:
    pop %eax
    pop %esi
    pop %edi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret