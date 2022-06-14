    .global main
main:
    mov $0, %ebx
    mov $0, %ecx
cycle:
    call readi32
    jc end
    test $1, %eax
    jz even
    add %eax, %ecx
    jmp cycle
even:
    add %eax, %ebx
    jmp cycle
end:
    mov %ebx, %eax
    call writei32
    mov %ecx, %eax
    call writei32
    call finish
