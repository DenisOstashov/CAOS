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
    struct stat lst, st;
    for (int i = 1; i < argc; ++i) {
        if (lstat(argv[i], &lst) == -1) {
            printf("%s (missing)\n", argv[i]);
        } else if (S_ISLNK(lst.st_mode) && stat(argv[i], &st) == -1) {
            printf("%s (broken symlink)\n", argv[i]);
        } else {
            printf("%s\n", argv[i]);
        }
    }
}