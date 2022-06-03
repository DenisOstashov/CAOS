#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "inttypes.h"
#include "fcntl.h"

volatile sig_atomic_t read_ind = 0;
volatile sig_atomic_t need_exit = 0;

enum {
    BLOCK_SIZE = 16,
};

void term_handler(int signum) {
    need_exit = 1;
    read_ind = 0;
}

void read_handler(int signum) {
    read_ind = signum - SIGRTMIN + 1;
}

int main(int argc, char **argv) {
    int fd_storage[argc];
    int64_t sum_storage[argc];
    struct sigaction sa_;
    sa_.sa_flags = 0;
    sa_.sa_handler = read_handler;
    sigemptyset(&sa_.sa_mask);
    sigset_t sigset_empty, sigset_blocked;
    sigemptyset(&sigset_blocked);
    sigemptyset(&sigset_empty);
    for (int i = 1; i < argc; ++i) {
        fd_storage[i] = open(argv[i], O_RDONLY);
        sum_storage[i] = 0;
        sigaction(SIGRTMIN + i - 1, &sa_, NULL);
        sigaddset(&sigset_blocked, SIGRTMIN + i - 1);
    }
    struct sigaction term_sa;
    term_sa.sa_flags = 0;
    term_sa.sa_handler = term_handler;
    sigemptyset(&term_sa.sa_mask);
    sigaction(SIGTERM, &term_sa, NULL);
    sigaddset(&sigset_blocked, SIGTERM);
    int64_t value = 0;
    char buf[BLOCK_SIZE];
    sigprocmask(SIG_BLOCK, &sigset_blocked, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (!need_exit) {
        while (!read_ind && !need_exit) {
            sigsuspend(&sigset_empty);
        }
        while (!need_exit && read_ind) {
            volatile int ind = read_ind;
            sigprocmask(SIG_UNBLOCK, &sigset_blocked, NULL);
            while (!need_exit && ind == read_ind) {
                if (fd_storage[ind] < 0) {
                    if (ind == read_ind) {
                        read_ind = 0;
                    }
                    break;
                }
                int res = read(fd_storage[ind], buf, BLOCK_SIZE);
                if (res == 0) {
                    close(fd_storage[ind]);
                    fd_storage[ind] = -1;
                    if (ind == read_ind) {
                        read_ind = 0;
                    }
                } else if (res == BLOCK_SIZE) {
                    sscanf(buf, "%" SCNd64 "\n", &value);
                    sum_storage[ind] += value;
                }
            }
            sigprocmask(SIG_BLOCK, &sigset_blocked, NULL);
        }
    }
    for (int i = 1; i < argc; ++i) {
        if (fd_storage[i] >= 0) {
            close(fd_storage[i]);
        }
        printf("%" PRId64 "\n", sum_storage[i]);
    }
    fflush(stdout);
    return 0;
}