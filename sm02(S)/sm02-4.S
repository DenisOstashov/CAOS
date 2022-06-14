    .text
    .global main
main:
    mov $pers_stack, %ebx
read:
    call readi32
    jc exit
    test %eax, %eax
    jl pop
    add $0x8, %ebx
    lea pers_stack(, %eax, 8), %ecx
    mov %ecx, 4(%ebx)
    call readi32
    mov %eax, (%ebx)
    jmp read
pop:
    neg %eax
    lea pers_stack(, %eax, 8), %ecx
    mov 4(%ecx), %edx
    test %edx, %edx
    jz empty
    add $0x8, %ebx
    mov (%ecx), %eax
    call writei32
    mov (%edx), %eax
    mov %eax, (%ebx)
    mov 4(%edx), %eax
    mov %eax, 4(%ebx)
    jmp read
empty:
    mov $0xffffffff, %eax
    call writei32
    jmp read
exit:
    call finish
    .data
pers_stack: .skip 0x2000
