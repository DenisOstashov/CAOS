#include "unistd.h"
#include "stdio.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/wait.h"

int main() {
    pid_t pid, parent = getpid();
    int wstatus, value;
    while (scanf("%d", &value) == 1) {
        if ((pid = fork()) == -1) {
            if (getpid() == parent) {
                printf("-1\n");
                return 0;
            }
            return 1;
        }
        if (pid != 0) {
            waitpid(pid, &wstatus, 0);
            if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0) {
                if (getpid() == parent) {
                    printf("-1\n");
                    return 0;
                }
                return 1;
            } else {
                printf("%d\n", value);
                return 0;
            }
        }
    }
    return 0;
}