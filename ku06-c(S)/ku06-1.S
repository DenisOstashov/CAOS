    .text
    .global run_on_stack
run_on_stack:
    push %ebp
    mov %esp, %ebp
    push %ebx
    push %esi
    push %edi
    movl 16(%ebp), %edx
    movl 12(%ebp), %ecx
    movl 8(%ebp), %eax
    mov %esp, %esi
    movl %edx, %esp
    sub $4, %esp
    push %ecx
    call *%eax
    mov %esi, %esp
    pop %edi
    pop %esi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret