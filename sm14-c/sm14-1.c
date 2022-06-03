#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

enum {
    MAX = 5,
};

volatile int counter = 0;

void handler(int signum) {
    if (++counter == MAX) {
        exit(0);
    }
    printf("%d\n", counter - 1);
    fflush(stdout);
}

int main() {
    struct sigaction sa = {};
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
    return 1;
}