#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "unistd.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/wait.h"

int64_t generate_or_fail(size_t i);

int64_t* parallel_populate(size_t N) {
    size_t counter = 0;
    pid_t pid[N];
    const size_t table_size = sizeof(int64_t) * N;
    int64_t *table = mmap(NULL, table_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    while (counter < N) {
        if ((pid[counter++] = fork()) == 0) {
            table[counter - 1] = generate_or_fail(counter - 1);//На кр в скобке забыл -1 :(
            _exit(0);
        }
    }
    int wstatus;
    for (size_t i = 0; i < N; ++i) {
        waitpid(pid[i], &wstatus, 0);
        if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0) {
            table[i] = -1;
        }
    }
    return table;
}