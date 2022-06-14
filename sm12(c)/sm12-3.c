#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"
#include "sys/wait.h"

int work(char *cmd) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (pid == -1) {
        return 1;
    } else {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        return WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 0;
    }
}

int main(int argc, char** argv) {
    return !((work(argv[1]) || work(argv[2])) && work(argv[3]));
}