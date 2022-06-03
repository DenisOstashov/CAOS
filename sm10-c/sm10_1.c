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

char full_path[PATH_MAX];

int cmp(const void *left, const void *right) {
    return strcasecmp(*((char **)left), *((char **) right));
}

void work(DIR * dir, size_t pos) {
    struct dirent *dire;
    if ((dire = readdir(dir)) == NULL) {
        closedir(dir);
        return;
    }
    size_t size = 1;
    size_t capacity = 4;
    char** array = (char **) malloc(capacity * sizeof(char*));
    array[0] = (char *) calloc(strlen(dire->d_name) + 1, sizeof(char));
    strcpy(array[0], dire->d_name);
    while ((dire = readdir(dir))) {
        array[size] = (char *) calloc(strlen(dire->d_name) + 1, sizeof(char));
        strcpy(array[size], dire->d_name);
        ++size;
        if (size == capacity) {
            capacity *= 2;
            char** new_array = (char **) realloc(array, capacity * sizeof(char*));
            array = new_array;
        }
    }
    closedir(dir);
    qsort(array, size, sizeof(char *), &cmp);
    struct stat st;
    char *start = full_path + pos;
    size_t limit = PATH_MAX - pos;
    for (size_t i = 0; i < size; ++i) {
        size_t len = snprintf(start, limit, "/%s", array[i]);
        if (strcmp(".", array[i]) != 0 && strcmp("..", array[i]) != 0
            && lstat(full_path, &st) != -1 && S_ISDIR(st.st_mode)) {
            DIR* new_dir = opendir(full_path);
            if (new_dir != NULL) {
                printf("cd %s\n", array[i]);
                work(new_dir, pos + len);
                puts("cd ..");
            }
        }
    }
    for (size_t i = 0; i < size; ++i) {
        free(array[i]);
    }
    free(array);
}

int main(int argc, char **argv) {
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        return 0;
    }
    work(dir, snprintf(full_path, PATH_MAX, "%s", argv[1]));
    return 0;
}
