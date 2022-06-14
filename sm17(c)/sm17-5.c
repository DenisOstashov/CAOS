#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "limits.h"

enum {
    THREAD_AMOUNT = 100,
    ITEM_AMOUNT = 1000,
};

struct Item {
    struct Item *next;
    long long value;
};

struct thread_info {
    pthread_t thread_id;
    int thread_num;
};

_Atomic (struct Item *) tail;

void *thread_start(void *arg) {
    struct thread_info *ptr = (struct thread_info *) arg;
    for (int i = 0; i < ITEM_AMOUNT; ++i) {
        struct Item * new_tail = (struct Item *) malloc(sizeof(struct Item));
        new_tail->value = i + ITEM_AMOUNT * (ptr->thread_num);
        new_tail->next = NULL;
        struct Item * expected = tail;
        while (!atomic_compare_exchange_strong(&tail, &expected, new_tail)) {}
        expected->next = new_tail;
        sched_yield();
    }
    return NULL;
}

int main() {
    struct Item head = {0};
    tail = &head;
    struct thread_info info[THREAD_AMOUNT];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
    for (int i = 0; i < THREAD_AMOUNT; ++i) {
        info[i].thread_num = i;
        pthread_create(&info[i].thread_id, &attr, thread_start, info + i);
    }
    pthread_attr_destroy(&attr);
    for (int i = 0; i < THREAD_AMOUNT; ++i) {
        pthread_join(info[i].thread_id, NULL);
    }
    struct Item *head_ptr = head.next;
    while (head_ptr) {
        printf("%lld\n", head_ptr->value);
        struct Item *tmp = head_ptr;
        head_ptr = head_ptr->next;
        free(tmp);
    }
}