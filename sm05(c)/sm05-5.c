#include <unistd.h>

enum {
    BUF_SIZE = 4096,
};

ssize_t copy_file(int in_fd, int out_fd) {
    ssize_t read_ret, success = 0;
    unsigned char buffer[BUF_SIZE];
    while ((read_ret = read(in_fd, buffer, BUF_SIZE))) {
        if (read_ret == -1) {
            return read_ret;
        }
        success += read_ret;
        ssize_t write_ret;
        unsigned char *head = buffer;
        while ((write_ret = write(out_fd, head, read_ret))) {
            if (write_ret == -1) {
                return write_ret;
            }
            read_ret -= write_ret;
            head += write_ret;
        }
    }
    return success;
}