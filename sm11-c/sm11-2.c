#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "unistd.h"
#include "errno.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))

int main(int argc, char **argv) {
    if (argc < 4) {
        return 0;
    }
    char *ptr;
    long begin = strtol(argv[2], &ptr, 10);
    if (!ptr || *ptr || begin <= 0 || errno == ERANGE) {
        fprintf(stderr, "Wrong number\n");
        return 1;
    }
    long end = strtol(argv[3], &ptr, 10);
    if (!ptr || *ptr || end <= 0 || errno == ERANGE) {
        fprintf(stderr, "Wrong number\n");
        return 1;
    }
    int32_t fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Can't open file\n");
        return 1;
    }
    struct stat statbuf;
    if (fstat(fd, &statbuf) == -1) {
        close(fd);
        fprintf(stderr, "Can't get file size\n");
        return 1;
    }
    int64_t size = statbuf.st_size;
    if (size == 0 || begin >= end) {
        close(fd);
        return 0;
    }
    char *table = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (table == MAP_FAILED) {
        fprintf(stderr, "Failed to read\n");
        close(fd);
        return 1;
    }
    int64_t line_max = end - begin + 1;
    int64_t capacity = 8;
    int64_t *limits = (int64_t *)calloc(capacity, sizeof(int64_t));
    int64_t ind = 0, counter = 1, line_amount = 0;
    if (begin == counter) {
        limits[line_amount] = -1;
        ++line_amount;
    }
    while (ind < size && line_amount < line_max) {
        if (table[ind] == '\n') {
            ++counter;
            if (counter >= begin) {
                limits[line_amount] = ind;
                ++line_amount;
                if (line_amount == capacity && line_amount < line_max) {
                    capacity = MIN(2 * capacity, line_max);
                    int64_t *new_limits = (int64_t *) realloc(limits, sizeof(int64_t) * capacity);
                    if (new_limits == NULL) {
                        free(limits);
                        munmap(table, size);
                        close(fd);
                        fprintf(stderr, "Realloc error\n");
                        return 1;
                    }
                    limits = new_limits;
                }
            }
        }
        ++ind;
    }
    if (counter >= begin && line_amount < line_max && table[size - 1] != '\n') {
        limits[line_amount] = size;
        ++line_amount;
    }
    if (line_amount >= 2) {
        for (int64_t line = line_amount - 1; line > 0; --line) {
            int64_t to_write = limits[line] - limits[line - 1] - 1;
            int64_t start = limits[line - 1] + 1;
            fwrite(table + start, sizeof(char), to_write, stdout);
            printf("\n");
        }
    }
    free(limits);
    if (munmap(table, size) == -1) {
        fprintf(stderr, "Unmap error\n");
        close(fd);
        return 1;
    }
    if (close(fd)) {
        fprintf(stderr, "Close error\n");
        return 1;
    }
    return 0;
}
