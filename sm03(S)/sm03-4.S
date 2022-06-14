#include "sys/syscall.h"
    .text
    .equ stdin, 0
    .equ stdout, 1
    .global main
main:
    push %ebp
    mov %esp, %ebp
    push %ebx
    mov $SYS_read, %eax
    mov $stdin, %ebx
    lea symbol, %ecx
    mov $0x1, %edx
read:
    int $0x80
    cmp %edx, %eax
    jne end
    mov $SYS_read, %eax
    incl symbols
    cmpb $0xa, (%ecx)
    je line
    cmpb $0x20, (%ecx)
    je space
    push %eax
    movb flag, %al
    test %al, %al
    jnz continue
    incl words
    movb $0x1, flag
continue:
    pop %eax
    jmp read
space:
    movb $0x0, flag
    jmp read    
line:
    incl lines
    movb $0x0, flag
    jmp read
end:
    mov lines, %eax
    call writei32
    mov words, %eax
    call writei32
    mov symbols, %eax
    call writei32
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    call finish


    .data
symbol: .byte 0x0
flag: .byte 0x0
lines: .int 0x0
words: .int 0x0
symbols: .int 0x0
