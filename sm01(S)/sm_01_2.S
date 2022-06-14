    .global main
main:
    call readi32
cycle:
    test %eax, %eax
    jl end
    call writei32
    sub $1, %eax
    jmp cycle
end:
    call finish
