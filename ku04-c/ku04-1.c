#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "unistd.h"
#include <math.h>
#include <errno.h>

int main(int argc, char **argv) {
    if (argc < 4) {
        perror("Not enough arguments");
        return 1;
    }
    size_t rows, cols;
    if (sscanf(argv[2], "%zu", &rows) < 1 || sscanf(argv[3], "%zu", &cols) < 1) {
        perror("Not a number");
        return 1;
    }
    int32_t fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Can't create file");
        return 1;
    }
    size_t table_size = sizeof(double) * rows * cols;
    if (ftruncate(fd, table_size) == -1) {
        perror("Truncate failed");
        close(fd);
        return 1;
    }
    double *table = mmap(NULL, table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (table == MAP_FAILED) {
        perror("Failed to read");
        close(fd);
        return 1;
    }
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            table[i * cols + j] = 2 * sin((double)i) + 4*cos((double)j/2);
        }
    }
    if (munmap(table, table_size) == -1) {
        perror("unmap error");
        close(fd);
        return 1;
    }
    close(fd);
}