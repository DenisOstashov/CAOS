#include <stdio.h>
#include <pthread.h>

enum {
    THREAD_AMOUNT = 10,
};

struct thread_info {
    pthread_t thread_id;
    int thread_num;
};

void *thread_start(void *arg) {
    struct thread_info *ptr = (struct thread_info *) arg;
    if (ptr->thread_num) {
        pthread_join(ptr->thread_id, NULL);
    }
    printf("%d\n", ptr->thread_num);
    return NULL;
}

int main() {
    struct thread_info info[THREAD_AMOUNT];
    pthread_t last;
    for (int i = 0; i < THREAD_AMOUNT; ++i) {
        info[i].thread_num = i;
        if (i) {
            info[i].thread_id = last;
        }
        pthread_create(&last, NULL, thread_start, info + i);
    }
    pthread_join(last, NULL);
}