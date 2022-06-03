#include "sys/syscall.h"
    .text

    .equ stdin, 0
    .equ stdout, 1

printnl:
    push %ebp
    mov %esp, %ebp
    push %ecx
    push %ebx
    push %eax
    mov $SYS_write, %eax
    mov $stdout, %ebx
    lea buffer, %ecx
    int $0x80
    pop %eax
    pop %ebx
    pop %ecx
    mov %ebp, %esp
    pop %ebp
    xor %edi, %edi
    ret

    .global main
main:
    push %edi
    push %ebx
    dec %esp
    movb $0x0, (%esp)
    xor %edi, %edi
read:
    mov $SYS_read, %eax
    mov $stdin, %ebx
    mov %esp, %ecx
    mov $0x1, %edx
    int $0x80
    cmp %edx, %eax
    jne end
    cmp $'\n', (%ecx)
    je reset
    cmp $'\n', %edi
    jl write
    call printnl
write:
    inc %edi
    mov $SYS_write, %eax
    mov $stdout, %ebx
    mov %esp, %ecx
    mov $0x1, %edx
    int $0x80
    jmp read
reset:
    call printnl
    mov $SYS_read, %eax
    mov $stdin, %ebx
    jmp read
end:
    test %edi, %edi
    jz finish
    call printnl
finish:
    inc %esp
    pop %ebx
    pop %edi
    mov $SYS_exit, %eax
    xor %ebx, %ebx
    int $0x80
buffer: .byte '\n'
