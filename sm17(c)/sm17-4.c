#include "stdio.h"
#include "pthread.h"
#include "inttypes.h"

pthread_mutex_t mutex;
pthread_cond_t cond;
uint64_t new_prime;
uint64_t base;
int found_prime = 0;

int prime_check(uint64_t value) {
    if (value % 2 == 0) {
        return value == 2;
    }
    for (uint64_t i = 3; i * i <= value; i += 2) {
        if (value % i == 0) {
            return 0;
        }
    }
    return value != 1;
}

void *thread_start(void *arg) {
    while (1) {
        if (prime_check(base)) {
            pthread_mutex_lock(&mutex);
            new_prime = base;
            found_prime = 1;
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&cond);
        }
        ++base;
    }
    return NULL;
}

int main() {
    uint32_t count;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t pthread;
    if (scanf("%" SCNu64 " %" SCNu32, &base, &count) != 2) {
        return 1;
    }
    pthread_create(&pthread, NULL, thread_start, NULL);
    for (int32_t i = 0; i < count; ++i) {
        pthread_mutex_lock(&mutex);
        while (found_prime == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        uint64_t prime = new_prime;
        found_prime = 0;
        pthread_mutex_unlock(&mutex);
        printf("%" PRIu64 "\n", prime);
    }
    /*pthread_cond_init(&cond, NULL);
    pthread_mutex_destroy(&mutex);*/
}
