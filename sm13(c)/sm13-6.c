#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"
#include "stdlib.h"
#include "inttypes.h"
#include "string.h"
#include "sys/wait.h"
#include "limits.h"
#include "ctype.h"

void destroy(pid_t *children, int start, int finish) {
    for (; start < finish; ++start) {
        kill(children[start], SIGKILL);
        waitpid(children[start], NULL, 0);
    }
    _exit(1);
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return 0;
    }
    int channels[argc - 1][2], wstatus, input = STDIN_FILENO;
    pid_t children[argc];
    for (int i = 1; i < argc - 1; ++i) {
        if (pipe(channels[i]) == -1) {
            destroy(children, 1, i);
        }
        if ((children[i] = fork()) == 0) {
            if (input != STDIN_FILENO) {
                dup2(input, STDIN_FILENO);
                close(input);
            }
            dup2(channels[i][1], STDOUT_FILENO);
            close(channels[i][1]);
            close(channels[i][0]);
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        if (input != STDIN_FILENO) {
            close(input);
        }
        close(channels[i][1]);
        input = channels[i][0];
        if (children[i] == -1) {
            close(channels[i][0]);
            destroy(children, 1, i);
        }
    }
    if ((children[argc - 1] = fork()) == 0) {
        if (input != STDIN_FILENO) {
            dup2(input, STDIN_FILENO);
            close(input);
        }
        execlp(argv[argc - 1], argv[argc - 1], NULL);
        _exit(1);
    }
    if (input != STDIN_FILENO) {
        close(input);
    }
    if (children[argc - 1] == -1) {
        destroy(children, 1, argc - 1);
    }
    for (int i = 1; i < argc; ++i) {
        waitpid(children[i], &wstatus, 0);
    }
    return 0;
}