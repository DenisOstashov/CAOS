#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "unistd.h"

struct direction {
    int32_t y;
    int32_t x;
};

struct borders {
    size_t right;
    size_t left;
    size_t up;
    size_t down;
};

int main(int argc, char **argv) {
    if (argc < 4) {
        puts("Not enough arguments");
        return 1;
    }
    size_t rows, cols;
    if (sscanf(argv[2], "%zu", &rows) < 1 || sscanf(argv[3], "%zu", &cols) < 1) {
        puts("Not a number");
        return 2;
    }
    int32_t fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
    if (fd < 0) {
        puts("Can't create file");
        return 3;
    }
    size_t table_size = sizeof(int32_t) * rows * cols;
    if (ftruncate(fd, table_size) == -1) {
        puts("Truncate failed");
        close(fd);
        return 4;
    } else if (!table_size) {
        close(fd);
        return 0;
    }
    int32_t *table = mmap(NULL, table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (table == MAP_FAILED) {
        puts("Failed to read");
        close(fd);
        return 5;
    }
    struct direction right = {0, 1}, left = {0, -1}, up = {-1, 0}, down = {1, 0};
    struct direction directed = right;
    struct borders border = {cols - 1, 0, 0, rows - 1};
    for (int32_t cur_num = 1, cur_row = 0, cur_col = 0; cur_num <= rows * cols; ++cur_num) {
        table[cur_row * cols + cur_col] = cur_num;
        if (cur_col == border.right && directed.x == 1) {
            directed = down;
            border.up += 1;
        } else if (cur_col == border.left && directed.x == -1) {
            directed = up;
            border.down -= 1;
        } else if (cur_row == border.down && directed.y == 1) {
            directed = left;
            border.right -= 1;
        } else if (cur_row == border.up && directed.y == -1) {
            directed = right;
            border.left += 1;
        }
        cur_row += directed.y;
        cur_col += directed.x;
    }
    if (munmap(table, table_size) == -1) {
        puts("unmap error");
        close(fd);
        return 6;
    }
    close(fd);
}