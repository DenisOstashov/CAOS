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

int main(int argc, char** argv) {
    int channel[2];
    pipe(channel);
    pid_t first, second;
    if ((first = fork()) == 0) {
        dup2(channel[1], STDOUT_FILENO);
        close(channel[1]);
        close(channel[0]);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    close(channel[1]);
    if ((second = fork()) == 0) {
        dup2(channel[0], STDIN_FILENO);
        close(channel[0]);
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }
    close(channel[0]);
    wait(NULL);
    wait(NULL);
}