#include "unistd.h"
#include "stdio.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/wait.h"

int main() {
    pid_t son_pid;
    int wstatus;
    if ((son_pid = fork()) == 0) {
        pid_t grandson_pid;
        if ((grandson_pid = fork()) == 0) {
            printf("3 ");
            return 0;
        } else if (grandson_pid == -1) {
            perror("fork error");
            return 1;
        }
        waitpid(grandson_pid, &wstatus, 0);
        printf("2 ");
        return 0;
    } else if (son_pid == -1) {
        perror("fork error");
        return 1;
    }
    waitpid(son_pid, &wstatus, 0);
    if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0) {
        return 1;
    }
    printf("1\n");
    return 0;
}