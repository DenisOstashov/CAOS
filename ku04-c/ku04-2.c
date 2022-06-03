#include "unistd.h"
#include "stdio.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "inttypes.h"
#include "stdio.h"
#include "stdlib.h"
/*

uint8_t frobnicate(uint8_t a, uint8_t b) {
    return a + b;
}
*/


int main(int argc, char **argv) {
    char *ptr;
    const int N = strtol(argv[1], &ptr, 10);
    pid_t first = 0, second = 0, batya = getpid();
    int counter = 0;
    int fwstatus, swstatus;
    uint8_t xyzzy;
    while ((++counter && (first = fork()) == 0) || (++counter && (second = fork()) == 0)) {
        if ((counter + 1) / 2 == N) {
            return counter;
        }
        if (counter % 2 == 0) {
            continue;
        }
        return counter;
    }
    waitpid(first, &fwstatus, 0);
    waitpid(second, &swstatus, 0);
    xyzzy = frobnicate(WEXITSTATUS(fwstatus), WEXITSTATUS(swstatus));
    if (getpid() == batya) {
        printf("%" PRIu8 "\n", xyzzy);
        return 0;
    } else {
        return xyzzy;
    }
}