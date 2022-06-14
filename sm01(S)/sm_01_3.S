    .global main
main:
    call readi64
    shl $1, %eax
    rcl $1, %edx
    mov %eax, %ebx
    mov %edx, %ecx
    call readi64
    shr $1, %edx
    rcr $1, %eax
    sub %eax, %ebx
    sbb %edx, %ecx
    mov %ebx, %eax
    mov %ecx, %edx
    call writei64
    call finish
