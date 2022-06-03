#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "inttypes.h"

volatile sig_atomic_t counter_1 = 0;
volatile sig_atomic_t counter_2 = 0;
volatile sig_atomic_t need_print = 0;

void handler_1(int signum) {
    need_print = 1;
}

void handler_2(int signum) {
    ++counter_2;
}

void handler_3(int signum) {
    need_print = -1;
}

int main() {
    struct sigaction sa_1 = {}, sa_2 = {}, sa_3;
    sa_1.sa_flags = SA_RESTART;
    sa_1.sa_handler = handler_1;
    sigemptyset(&sa_1.sa_mask);
    sigaction(SIGUSR1, &sa_1, NULL);
    sa_2.sa_flags = SA_RESTART;
    sa_2.sa_handler = handler_2;
    sigemptyset(&sa_2.sa_mask);
    sigaction(SIGUSR2, &sa_2, NULL);
    sa_3.sa_flags = SA_RESTART;
    sa_3.sa_handler = handler_3;
    sigemptyset(&sa_3.sa_mask);
    sigaction(SIGTERM, &sa_3, NULL);
    sigset_t sigset_empty, sigset_blocked;
    sigemptyset(&sigset_blocked);
    sigaddset(&sigset_blocked, SIGUSR1);
    sigaddset(&sigset_blocked, SIGUSR2);
    sigaddset(&sigset_blocked, SIGTERM);
    sigprocmask(SIG_BLOCK, &sigset_blocked, &sigset_empty);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        while (!need_print) {
            sigsuspend(&sigset_empty);
        }
        if (need_print == -1) {
            return 0;
        }
        printf("%d %d\n", counter_1, counter_2);
        fflush(stdout);
        ++counter_1;
        need_print = 0;
    }
    return 0;
}