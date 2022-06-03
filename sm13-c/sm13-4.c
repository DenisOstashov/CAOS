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
    int channel[2], wstatus;
    int32_t value;
    pipe(channel);
    pid_t first, second;
    if ((first = fork()) == 0) {
        close(channel[1]);
        if ((second = fork()) == 0) {
            int64_t ans = 0;
            while (read(channel[0], &value, sizeof(int32_t)) > 0) {
                ans += value;
            }
            close(channel[0]);
            printf("%" PRId64 "\n", ans);
            return 0;
        }
        close(channel[0]);
        waitpid(second, &wstatus, 0);
        return 0;
    }
    close(channel[0]);
    while (scanf("%" SCNd32 "", &value) == 1) {
        write(channel[1], &value, sizeof(int32_t));
    }
    close(channel[1]);
    waitpid(first, &wstatus, 0);
}