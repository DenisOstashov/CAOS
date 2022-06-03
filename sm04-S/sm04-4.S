#include "sys/syscall.h"
    .text
    .global print_tb
print_tb:
    push %ebp
    mov %esp, %ebp
    push %ebx
    push %esi
    push %edi
    mov $first, %eax
    movl $main, (%eax)
    movl $main_, 4(%eax)
    movl $5, 8(%eax)
    mov $second, %eax
    movl $foo, (%eax)
    movl $foo_, 4(%eax)
    movl $4, 8(%eax)
    mov $third, %eax
    movl $bar, (%eax)
    movl $bar_, 4(%eax)
    movl $4, 8(%eax)
    push $second
    push $first
    call comp_and_swap
    add $8, %esp
    push $third
    push $first
    call comp_and_swap
    add $4, %esp
    push $second
    call comp_and_swap
    add $8, %esp
    mov $SYS_write, %eax
    mov $0x1, %ebx
    mov $print_tb_, %ecx
    mov $9, %edx
    int $0x80
    mov %ebp, %edi
cycle:
    mov 4(%edi), %esi
    cmp second, %esi
    jl pr_first
    cmp third, %esi
    jl pr_second
    push $third
    call pr_func
    jmp next
pr_first:
    push $first
    call pr_func
    jmp next
pr_second:
    push $second
    call pr_func
next:
    pop %eax
    mov (%eax), %eax
    cmp $main, %eax
    je end
    mov (%edi), %edi
    jmp cycle
end:
    pop %edi
    pop %esi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret

comp_and_swap:
    push %ebp
    mov %esp, %ebp
    push %ebx
    mov 8(%ebp), %ebx
    mov (%ebx), %ecx
    mov 12(%ebp), %eax
    mov (%eax), %edx
    cmp %edx, %ecx
    jl first_less
    mov %edx, (%ebx)
    mov %ecx, (%eax)
    mov 4(%ebx), %ecx
    mov 4(%eax), %edx
    mov %edx, 4(%ebx)
    mov %ecx, 4(%eax)
    mov 8(%ebx), %ecx
    mov 8(%eax), %edx
    mov %edx, 8(%ebx)
    mov %ecx, 8(%eax)
first_less:
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret

pr_func:
    push %ebp
    mov %esp, %ebp
    push %ebx
    mov 8(%ebp), %eax
    mov $1, %ebx
    mov 4(%eax), %ecx
    mov 8(%eax), %edx
    mov $SYS_write, %eax
    int $0x80
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret

    .data
first:
    .int 0
    .int 0
    .int 0
second:
    .int 0
    .int 0
    .int 0
third:
    .int 0
    .int 0
    .int 0
print_tb_: .ascii "print_tb\n"
foo_: .ascii "foo\n"
bar_: .ascii "bar\n"
main_: .ascii "main\n"
