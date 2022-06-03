#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <sys/types.h>
#include <dirent.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/mman.h>

char full_path[PATH_MAX];

int pref(FILE *file, char *preff) {
    for (int i = 0; preff[i]; ++i) {
        int res = fgetc(file);
        if (res == EOF || ((unsigned char)res != (unsigned char)preff[i])) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv) {
    DIR *dir = opendir(argv[1]);
    if (!dir) {
        return 0;
    }
    size_t pos = snprintf(full_path, PATH_MAX, "%s", argv[1]);
    struct dirent *dire;
    char *start = full_path + pos;
    size_t limit = PATH_MAX - pos;
    struct stat st;
    while ((dire = readdir(dir))) {
        snprintf(start, limit, "/%s", dire->d_name);
        if (stat(full_path, &st) == -1) {
            continue;
        }
        if (S_ISREG(st.st_mode)) {
            FILE *file = fopen(full_path, "r");
            if (file == NULL) {
                continue;
            }
            if (pref(file, argv[2])) {
                printf("%s\n", dire->d_name);
            }
            fclose(file);
        }
    }
    closedir(dir);
}