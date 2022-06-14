    .global main
main:
    xor %ebx, %ebx
cycle:
    call readi32
    jc end
    add %eax, %ebx
    jno cycle
    mov $0x7fffffff, %ebx
    adc $0, %ebx
end:
    mov %ebx, %eax
    call writei32
    call finish
