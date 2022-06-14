#include "sys/syscall.h"
    .text
    .equ stdin, 0
    .equ stdout, 1
    .global writeu32
writeu32:
    push %ebp
    mov %esp, %ebp
    sub $0x4, %esp
    push %ebx
    push %edi
    push %esi
    push $1
    push $10
    push $100
    push $1000
    push $10000
    push $100000
    push $1000000
    push $10000000
    push $100000000
    push $1000000000
    mov $0xa, %eax
    xor %esi, %esi
cycle:
    movb $0x30, -4(%ebp)
    mov %ecx, %ebx
    pop %edi
my_sub:
    sub %edi, %ebx
    jc zero_check
    mov %ebx, %ecx
    incb -4(%ebp)
    jmp my_sub
zero_check:
    test %esi, %esi
    jnz output
    cmpb $0x30, -4(%ebp)
    je prepare
    inc %esi
output:
    push %ecx
    lea -4(%ebp), %ecx
    call safe_print
    pop %ecx
prepare:
    dec %eax
    jnz cycle
    test %esi, %esi
    jnz end
    lea -4(%ebp), %ecx
    call safe_print
end:
    pop %esi
    pop %edi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret

safe_print:
    push %ebp
    mov %esp, %ebp
    push %eax
    push %ebx
    mov $SYS_write, %eax
    mov $stdout, %ebx
    mov $0x1, %edx
    int $0x80
    pop %ebx
    pop %eax
    mov %ebp, %esp
    pop %ebp
    ret
