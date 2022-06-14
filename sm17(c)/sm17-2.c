#include <stdio.h>
#include <pthread.h>

void *thread_start(void *arg) {
    int value;
    if (scanf("%d", &value) == 1) {
        pthread_t new_thread;
        pthread_create(&new_thread, NULL, thread_start, NULL);
        pthread_join(new_thread, NULL);
        printf("%d\n", value);
    }
    return NULL;
}

int main() {
    pthread_t first;
    pthread_create(&first, NULL, thread_start, NULL);
    pthread_join(first, NULL);
}