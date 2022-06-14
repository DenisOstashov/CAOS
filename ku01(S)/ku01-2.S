#include "sys/syscall.h"
    .text
    .equ stdin, 0
    .equ stdout, 1
    .global main
main:
    push %edi
    push %esi
    sub $4, %esp
cycle:
    mov $SYS_read, %eax
    mov $stdin, %ebx
    mov %esp, %ecx
    mov $0x1, %edx
    int $0x80
    cmpb $10, (%ecx)
    je next
    cmpb $32, (%ecx)
    je cycle
    xor %eax, %eax
    movb (%ecx), %al
    mov $first, %edx
    incl (%edx, %eax, 4)
    cmpb $65, (%ecx)
    jl smth
    cmpb $90, (%ecx)
    jg smth
    add $32, %eax
    incl (%edx, %eax, 4)
    jmp cycle
smth:
    cmpb $97, (%ecx)
    jl cycle
    cmpb $122, (%ecx)
    jg cycle
    sub $32, %eax
    incl (%edx, %eax, 4)
    jmp cycle
next:
    mov $SYS_read, %eax
    mov $stdin, %ebx
    mov %esp, %ecx
    mov $0x1, %edx
    int $0x80
    cmpb $10, (%ecx)
    je check
    cmpb $32, (%ecx)
    je next
    xor %eax, %eax
    movb (%ecx), %al
    mov $second, %edx
    incl (%edx, %eax, 4)
    cmpb $65, (%ecx)
    jl iop
    cmpb $90, (%ecx)
    jg iop
    add $32, %eax
    incl (%edx, %eax, 4)
    jmp next
iop:
    cmpb $97, (%ecx)
    jl next
    cmpb $122, (%ecx)
    jg next
    sub $32, %eax
    incl (%edx, %eax, 4)
    jmp next
check:
    mov $0, %eax
loop:
    mov first(, %eax, 4), %ecx
    mov second(, %eax, 4), %edx
    cmp %ecx, %edx
    jne sad
    inc %eax
    cmp $128, %eax
    je good
    jmp loop
sad:
    mov $SYS_write, %eax
    mov $stdout, %ebx
    lea no, %ecx
    mov $2, %edx
    int $0x80
    jmp end
good:
    mov $SYS_write, %eax
    mov $stdout, %ebx
    lea yes, %ecx
    mov $3, %edx
    int $0x80
    jmp end

end:
    add $4, %esp
    pop %esi
    pop %edi
    mov $SYS_exit, %eax
    xor %ebx, %ebx
    int $0x80
    .data
first: .skip 512, 0
second: .skip 512, 0
yes: .asciz "YES"
no: .asciz "NO"
