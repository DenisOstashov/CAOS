#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"
#include "sys/wait.h"
#include <string.h>
#include "stdlib.h"

int main(int argc, char** argv) {
    pid_t sons[2];
    int sep = 1;
    for (; sep < argc; ++sep) {
        if (strcmp(argv[sep], ";") == 0) {
            break;
        }
    }
/*    printf("%d\n", sep);*/
    char ** args_f = (char **) calloc(sep, sizeof(char*));
    memcpy(args_f, argv + 1, (sep - 1) * sizeof(char *));
    args_f[sep - 1] = NULL;
/*    for (int i = 0; i < sep - 1; ++i) {
        printf("%s\n", args_f[i]);
    }*/
    char ** args_s = (char **) calloc(argc - sep, sizeof(char*));
    memcpy(args_s, argv + sep + 1, (argc - sep - 1)* sizeof(char *));
    args_f[argc - sep - 1] = NULL;
/*
    for (int i = 0; i < argc - sep - 1; ++i) {
        printf("%s\n", args_s[i]);
    }*/
    if ((sons[0] = fork()) == 0) {
        execvp(args_f[0], args_f);
        _exit(1);
    }
    if ((sons[1] = fork()) == 0) {
        execvp(args_s[0], args_s);
        _exit(1);
    }
    pid_t waited = wait(NULL);
    if (sons[0] == waited) {
        kill(sons[1], SIGKILL);
    } else {
        kill(sons[0], SIGKILL);
    }
}