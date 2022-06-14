#define _GNU_SOURCE
#include "sched.h"
#include "stddef.h"
#include "sys/syscall.h"
#include "linux/futex.h"
#include "unistd.h"
#include "stdatomic.h"
#include "stdlib.h"

struct caos_thread_info {
    _Atomic int exit_flag;
    int exit_status;
    int (*func)(void*);
    void *arg;
    char *stack;
};

typedef struct caos_thread_info caos_thread_t;

int __shell_func(void *ptr) {
    caos_thread_t *caos_thread_ptr = (caos_thread_t *) ptr;
    caos_thread_ptr->exit_status = caos_thread_ptr->func(caos_thread_ptr->arg);
    atomic_exchange(&caos_thread_ptr->exit_flag, 1);
    syscall(SYS_futex, &caos_thread_ptr->exit_flag, FUTEX_WAKE, 1, NULL, NULL, 0);
    return caos_thread_ptr->exit_status;
}

caos_thread_t* caos_thread_create(int (*func)(void*), void *arg) {
    enum {
        STACK_SIZE = 1 << 20,
    };
    caos_thread_t *caos_thread_ptr = (caos_thread_t *) malloc(sizeof(caos_thread_t));
    caos_thread_ptr->stack = (char *) calloc(STACK_SIZE, sizeof(char));
    caos_thread_ptr->exit_flag = 0;
    caos_thread_ptr->func = func;
    caos_thread_ptr->arg = arg;
    clone(__shell_func, caos_thread_ptr->stack + STACK_SIZE, CLONE_THREAD | CLONE_VM | CLONE_SIGHAND, caos_thread_ptr);
    return caos_thread_ptr;
}


int caos_thread_retval(caos_thread_t *thread) {
    while (!thread->exit_flag) {
        syscall(SYS_futex, &thread->exit_flag, FUTEX_WAIT, 0, NULL, NULL, 0);
    }
    return thread->exit_status;
}