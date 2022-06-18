#include "inttypes.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"

#define close2(FD) {\
    close(FD[0]);   \
    close(FD[1]);   \
}

#define wrapper(cmd) { \
    if (cmd) {}        \
}

int main(int argc, char **argv) {
    if (argc < 3) {
        return 1;
    }
    int pipes[2][2];
    pid_t child[2];
    if (pipe(pipes[0]) == -1) {
        return 1;
    }
    if (pipe(pipes[1]) == -1) {
        close2(pipes[0]);
        return 1;
    }
    if ((child[0] = fork()) == 0) {
        close2(pipes[1])
        close(pipes[0][0]);
        int32_t sum = 0, num;
        FILE *file = fopen(argv[1], "r");
        if (file) {
            while (fscanf(file, "%" SCNd32, &num) == 1) {
                sum += num;
            }
        }
        if (write(pipes[0][1], &sum, sizeof(sum))) {}
        close(pipes[0][1]);
        return 0;
    }
    close(pipes[0][1]);
    if ((child[1] = fork()) == 0) {
        close(pipes[0][0]);
        close(pipes[1][0]);
        int32_t sum = 0, num;
        FILE *file = fopen(argv[2], "r");
        if (file) {
            while (fscanf(file, "%" SCNd32, &num) == 1) {
                sum += num;
            }
        }
        wrapper(write(pipes[1][1], &sum, sizeof(sum)))
        close(pipes[1][1]);
        return 0;
    }
    close(pipes[1][1]);
    int32_t res1, res2;
    wrapper(read(pipes[0][0], &res1, sizeof(res1)))
    close(pipes[0][0]);
    wrapper(read(pipes[1][0], &res2, sizeof(res2)))
    close(pipes[1][0]);
    wait(NULL);
    wait(NULL);
    printf("%" PRId32 "\n%" PRId32 "\n%"  PRId32 "\n", res2, res1, res1 + res2);
}