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

char interpreter_path[PATH_MAX];

void work(DIR * dir, size_t pos) {
    struct dirent *dire;
    char *start = full_path + pos;
    size_t limit = PATH_MAX - pos;
    struct stat st, interpreter_st;
    while ((dire = readdir(dir))) {
        size_t len = snprintf(start, limit, "/%s", dire->d_name);
        /*printf("%s %zu\n%s\n", dire->d_name, len, full_path);*/
        /*if (len > 4) {
            printf("%s %d\n", dire->d_name + len - 4, strcmp(dire->d_name + len - 4, ".py"));
        }*/
        if (len > 4 && strcmp(dire->d_name + len - 4, ".py") == 0 && stat(full_path, &st) == 0 &&
            S_ISREG(st.st_mode) && access(full_path, R_OK | X_OK) == 0) {
            int fd = open(full_path, O_RDONLY);
            size_t size = st.st_size;
            if (size <= 2 || fd < 0) {
                continue;
            }
            char *table = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (table == MAP_FAILED) {
                close(fd);
                continue;
            }
            if (table[0] == '#' && table[1] == '!') {
                size_t ind = 0;
                for (; ind < PATH_MAX && ind + 2 < size &&
                    table[ind + 2] != '\n' && table[ind + 2] != ' '; ++ind) {
                    interpreter_path[ind] = table[ind + 2];
                }
                if (ind < PATH_MAX && ind + 2 < size &&
                    (table[ind + 2] == ' ' || table[ind + 2] == '\n')) {
                    interpreter_path[ind] = '\0';
                    /*printf("%s\n", interpreter_path);
                    printf("%d %d %d\n", stat(interpreter_path, &interpreter_st),
                                         S_ISREG(interpreter_st.st_mode),
                                         access(interpreter_path, X_OK));*/
                    if (stat(interpreter_path, &interpreter_st) == 0 &&
                        S_ISREG(interpreter_st.st_mode) &&
                        access(interpreter_path, X_OK) == 0) {
                        printf("%s\n", full_path);
                    }
                }
            }
            munmap(table, size);
            close(fd);
        }
    }
}

int main(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        DIR *dir = opendir(argv[1]);
        if (dir != NULL) {
            work(dir, snprintf(full_path, PATH_MAX, "%s", argv[i]));
            closedir(dir);
        }
    }
    return 0;
}