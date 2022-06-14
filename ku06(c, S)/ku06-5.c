#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <sys/types.h>
#include <dirent.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>

int timer_check(struct epoll_event *events, int amount, int timer_fd) {
    for (int i = 0; i < amount; ++i) {
        if (events[i].data.fd == timer_fd) {
            return 1;
        }
    }
    return 0;
}

enum {
    BUF_SIZE = 1024,
    NS = 980000000,
};

int main(int argc, char **argv) {
    int timer_fd = timerfd_create(CLOCK_REALTIME, 0);
    struct itimerspec itmspec = {0};
    itmspec.it_value.tv_nsec = NS;
    timerfd_settime(timer_fd, 0, &itmspec, NULL);
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGTERM);
    sigprocmask(SIG_BLOCK, &set, NULL);
    int sig_fd = signalfd(-1, &set, 0);
    int epoll_fd = epoll_create1(0);
    int ret;
    struct epoll_event sig_evt = {0};
    sig_evt.data.fd = sig_fd;
    sig_evt.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sig_fd, &sig_evt);
    sig_evt.data.fd = STDIN_FILENO;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &sig_evt);
    sig_evt.data.fd = timer_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, timer_fd, &sig_evt);
    struct epoll_event events[3];
    struct signalfd_siginfo info;
    ssize_t success;
    char buf[BUF_SIZE];
    while (!timer_check(events, (ret = epoll_wait(epoll_fd, events, 3, -1)), timer_fd)) {
        if (ret == 2 || events[0].data.fd == sig_fd) {
            if (read(sig_fd, &info, sizeof(info)) < 0) {
                perror("read error\n");
                return 1;
            }
            printf("We communicate on my terms.\n");
            fflush(stdout);
        }
        if (ret == 2 || events[0].data.fd == STDIN_FILENO) {
            if ((success = read(STDIN_FILENO, buf, BUF_SIZE)) == -1) {
                perror("read error\n");
                return 1;
            }
            ssize_t old = success;
            char *ptr = buf;
            while (old) {
                if ((success = write(STDOUT_FILENO, ptr, old)) != -1) {
                    old -= success;
                    ptr += success;
                }
            }
            fflush(stdout);
        }
    }
    close(epoll_fd);
    close(sig_fd);
}