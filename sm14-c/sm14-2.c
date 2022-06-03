#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "inttypes.h"

volatile sig_atomic_t num = 1;

void handler_1(int signum) {
    num = 1;
}

void handler_2(int signum) {
    num = 2;
}

int main() {
    struct sigaction sa_1 = {}, sa_2 = {};
    sa_1.sa_flags = SA_RESTART;
    sa_1.sa_handler = handler_1;
    sigemptyset(&sa_1.sa_mask);
    sigaction(SIGUSR1, &sa_1, NULL);
    sa_2.sa_flags = SA_RESTART;
    sa_2.sa_handler = handler_2;
    sigemptyset(&sa_2.sa_mask);
    sigaction(SIGUSR2, &sa_2, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    volatile int64_t value;
    while (scanf("%" SCNd64 "", &value) == 1) {
        if (num & 1) {
            value = -value;
        } else {
            value *= value;
        }
        printf("%" PRId64 "\n", value);
        fflush(stdout);
    }
    return 0;
}