#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"
#include "sys/wait.h"

int mysys(const char *str) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    } else if (pid == -1) {
        return -1;
    } else {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        if (WIFSIGNALED(wstatus)) {
            return (int) 128 + WTERMSIG(wstatus);
        }
        return WEXITSTATUS(wstatus);
    }
}