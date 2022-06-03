#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "inttypes.h"
#include "fcntl.h"
#include <sys/signalfd.h>
#include <sys/epoll.h>
#include <sys/wait.h>

void work(int *channel, int num, int max) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);
    int sig_fd = signalfd(-1, &set, 0);
    int epoll_fd = epoll_create1(0);
    struct epoll_event sig_evt = {0};
    sig_evt.data.fd = sig_fd;
    sig_evt.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sig_fd, &sig_evt);
    FILE *in = fdopen(channel[0], "r");
    FILE *out = fdopen(channel[1], "w");
    pid_t brother_pid;
    struct epoll_event evt;
    struct signalfd_siginfo info;
    if (num == 1) {
        fprintf(out, "%d\n", getpid());
        fflush(out);
        epoll_wait(epoll_fd, &evt, 1, -1);
        if (fscanf(in, "%d", &brother_pid) != 1) {
            fprintf(stderr, "Can't get brother pid\n");
            _exit(1);
        }
        if (read(sig_fd, &info, sizeof(info)) < 0) {
            fprintf(stderr, "Can't handle signal\n");
            kill(brother_pid, SIGKILL);
            _exit(1);
        }
    } else {
        if (fscanf(in, "%d", &brother_pid) != 1) {
            fprintf(stderr, "Can't get brother pid\n");
            _exit(1);
        }
        fprintf(out, "%d\n", getpid());
        fflush(out);
        kill(brother_pid, SIGUSR1);
    }
    int value = 2 - num;
    if (num == 1) {
        printf("%d %d\n", num, value);
        fflush(stdout);
        fprintf(out, "%d\n", value + 1);
        fflush(out);
        kill(brother_pid, SIGUSR1);
    }
    while (value < max) {
        epoll_wait(epoll_fd, &evt, 1, -1);
        if (read(sig_fd, &info, sizeof(info)) < 0) {
            fprintf(stderr, "Can't handle signal\n");
            kill(brother_pid, SIGKILL);
            _exit(1);
        }
        if (fscanf(in, "%d", &value) != 1) {
            fprintf(stderr, "Can't get new value\n");
            kill(brother_pid, SIGKILL);
            _exit(1);
        }
        if (value <= max) {
            printf("%d %d\n", num, value);
            fflush(stdout);
            fprintf(out, "%d\n", value + 1);
            fflush(out);
            kill(brother_pid, SIGUSR1);
        }
    }
    close(epoll_fd);
    close(sig_fd);
    fclose(in);
    fclose(out);
    close(channel[1]);
    close(channel[2]);
    _exit(0);
}

int main(int argc, char **argv) {
    int channel[2], max;
    sscanf(argv[1], "%d" SCNd64, &max);
    if (max == 0) {
        return 0;
    }
    if (pipe(channel) < 0) {
        return 1;
    }
    pid_t first, second;
    if ((first = fork()) == 0) {
        work(channel, 1, max);
    }
    if ((second = fork()) == 0) {
        work(channel, 2, max);
    }
    close(channel[0]);
    close(channel[1]);
    waitpid(first, NULL, 0);
    waitpid(second, NULL, 0);
    return 0;
}