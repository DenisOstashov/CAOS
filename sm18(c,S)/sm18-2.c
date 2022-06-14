#include "stddef.h"
#include "stdatomic.h"

enum {
    WAITER_EXISTS = 42,
};

typedef _Atomic int caos_mutex_t;

void futex_wait(caos_mutex_t *addr, int val);

void futex_wake(caos_mutex_t *addr, int num);

void caos_mutex_init(caos_mutex_t *m) {
    atomic_exchange(m, 0);
}

void caos_mutex_unlock(caos_mutex_t *m) {
    if (atomic_exchange(m, 0) == WAITER_EXISTS) {
        futex_wake(m, 1);
    }
}

void caos_mutex_lock(caos_mutex_t *m) {
    int prev = 0;
    if (!atomic_compare_exchange_weak(m, &prev, 1)) {
        if (prev != WAITER_EXISTS) {
            prev = atomic_exchange(m, WAITER_EXISTS);
        }
        while (prev != 0) {
            futex_wait(m, WAITER_EXISTS);
            prev = atomic_exchange(m, WAITER_EXISTS);
        }
    }
}