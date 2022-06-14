#include <stdio.h>
#include <pthread.h>

enum {
    THREAD_AMOUNT = 3,
    OPERATION_AMOUNT = 1000000,
    HUNDRED = 100,
};

struct thread_info {
    pthread_t thread_id;
    int thread_num;
};

double array[THREAD_AMOUNT] = {0};
pthread_mutex_t mutex;

void *thread_start(void *arg) {
    struct thread_info *ptr = (struct thread_info *) arg;
    for (int i = 0; i < OPERATION_AMOUNT; ++i) {
        pthread_mutex_lock(&mutex);
        array[ptr->thread_num] += HUNDRED * (ptr->thread_num + 1);
        array[(ptr->thread_num + 1) % THREAD_AMOUNT] -= HUNDRED * (ptr->thread_num + 1) + 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    struct thread_info info[THREAD_AMOUNT];
    for (int i = 0; i < THREAD_AMOUNT; ++i) {
        info[i].thread_num = i;
        pthread_create(&info[i].thread_id, NULL, thread_start, info + i);
    }
    for (int i = 0; i < THREAD_AMOUNT; ++i) {
        pthread_join(info[i].thread_id, NULL);
    }
    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < THREAD_AMOUNT; ++i) {
        printf("%.10g\n", array[i]);
    }
}
