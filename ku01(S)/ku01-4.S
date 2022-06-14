#include "sys/syscall.h"
    .text
    .equ stdin, 0
    .equ stdout, 1
    .global main
main:
    push $hello
    lea -4(%esp), %ebp
    call process 
hello:
    call writei32
    call finish
