#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "unistd.h"

enum {
    HIGH_BIT = 0x80,
};

void compute(int64_t *value, int64_t *sum, int64_t *shift, uint64_t start, uint64_t size, int32_t fd) {
    unsigned char *table = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, start);
    for (uint64_t i = 0; i < size; ++i) {
        *value |= ((int64_t)(table[i] & ~HIGH_BIT)) << *shift;
        *shift += 7;
        if (table[i] & HIGH_BIT) {
            continue;
        }
        if (table[i] & (HIGH_BIT >> 1)) {
            *value |= (~((int64_t)0)) << *shift;
        }
        *sum += *value;
        *shift = 0;
        *value = 0;
    }
    munmap(table, size);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        return 0;
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
    uint64_t size = statbuf.st_size;
    if (size == 0) {
        printf("0\n0\n0\n");
        close(fd);
        return 0;
    }
    uint64_t left = 1, right = size + 1;
    while (left + 1 < right) {
        uint64_t mid = (right + left) / 2;
        unsigned char *table = mmap(NULL, mid, PROT_READ, MAP_PRIVATE, fd, 0);
        if (table == MAP_FAILED) {
            right = mid;
        } else {
            munmap(table, mid);//pls, stop
            left = mid;
        }
    }
    uint64_t start = 0;
    int64_t sum = 0;
    int64_t value = 0, shift = 0;
    while (left + start <= size) {
        compute(&value, &sum, &shift, start, left, fd);
        start += left;
    }
    if (start < size) {
        compute(&value, &sum, &shift, start, size - start, fd);
    }
    printf("%" PRIu64 "\n%" PRIu64 "\n%" PRId64 "\n", size, left, sum);
    close(fd);
}