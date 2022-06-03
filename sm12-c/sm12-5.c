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

enum {
    BASE_CAPACITY = 8,
    SMTH_WENT_WRONG = 2022,
};

char **parse_cmd(const char *cmd) {
    char **args = (char **)calloc(BASE_CAPACITY, sizeof(char*));
    size_t capacity = BASE_CAPACITY, size = 0, begin = 0, end;
    while (cmd[begin] != '\0') {
        if (isspace(cmd[begin])) {
            ++begin;
            continue;
        }
        if (cmd[begin] == '\0') {
            break;
        }
        for (end = begin + 1; cmd[end] != '\0' && !isspace(cmd[end]); ++end) {}
        args[size] = (char *)calloc(end - begin + 1, sizeof(char));
        for (size_t i = 0; i < end - begin; ++i) {
            args[size][i] = cmd[begin + i];
        }
        args[size][end - begin] = '\0';
        begin = end;
        if (++size == capacity) {
            capacity *= 2;
            args = (char **)realloc(args, sizeof(char *) * capacity);
        }
    }
    if (size == 0) {
        free(args);
        args = NULL;
    } else {
        args[size] = NULL;
    }
    return args;
}

int mysystem(const char *cmd) {
    char **args = parse_cmd(cmd);
    pid_t pid;
    if (args == NULL || (pid = fork()) == -1) {
        return -1;
    } else if (pid == 0) {
        execvp(args[0], args);
        _exit(1);
    }
    int wstatus;
    waitpid(pid, &wstatus, 0);
    if (WIFSIGNALED(wstatus)) {
        return WTERMSIG(wstatus) + 1024;
    }
    if (WIFEXITED(wstatus)) {
        return WEXITSTATUS(wstatus);
    }
    return SMTH_WENT_WRONG;
}

/*int main() {
    printf("%d\n", mysystem(""));
    printf("%d\n", mysystem("ls -l"));
}*/
