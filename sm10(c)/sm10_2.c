#include <sys/types.h>
#include <dirent.h>
#include <strings.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <inttypes.h>

int main(int argc, char **argv) {
    uint64_t result = 0;
    struct stat st;
    for (int i = 1; i < argc; ++i) {
        if (lstat(argv[i], &st) == 0 && S_ISREG(st.st_mode) && (st.st_nlink == 1)) {
            result += st.st_size;
        }
    }
    printf("%" PRIu64 "\n", result);
}