#include "sys/syscall.h"
    .text
    .equ stdin, 0
    .equ stdout, 1
skipb:
    push %ebp
    mov %esp, %ebp
    push %ebx
    mov $SYS_read, %eax
    mov $stdin, %ebx
    lea buffer, %ecx
    mov $0x1, %edx
    int $0x80
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret

    .global main
main:
    push %ebp
    mov %esp, %ebp
    push %ebx
    push %esi
    xor %esi, %esi
read:
    mov $SYS_read, %eax
    mov $stdin, %ebx
    lea buffer, %ecx
    mov $0x1, %edx
    int $0x80
    cmp %edx, %eax
    jne end
    incl symbols
    xor %eax, %eax
    movb buffer, %al
    test $0x80, %al
    jz read
    xor %ebx, %ebx
    movb $0x40, %bl
cycle:
    testb %bl, %al
    jz skip
    inc %esi
    shrb %bl
    jmp cycle
skip:
    test %esi, %esi
    jz read
    dec %esi
    call skipb
    jmp skip
end:
    movl symbols, %eax
    call writei32
    pop %esi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    call finish
    .data
buffer: .byte 0
symbols: .int 0
