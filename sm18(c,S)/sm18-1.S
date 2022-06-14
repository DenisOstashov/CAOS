    .text
    .global spin_lock, spin_unlock
spin_lock:
    push %ebp
    mov %esp, %ebp
    movl 8(%ebp), %ecx
    movl $1, %eax
loop:
    xchgl %eax, (%ecx)
    test %eax, %eax
    jz end
    pause
    jmp loop
end:
    mov %ebp, %esp
    pop %ebp
    ret

spin_unlock:
    push %ebp
    mov %esp, %ebp
    movl 8(%ebp), %ecx
    xor %eax, %eax
    xchgl %eax, (%ecx)
    mov %ebp, %esp
    pop %ebp
    ret
