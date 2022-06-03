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

const char *perms_to_str(char *buf, size_t size, int perms) {
    if (size == 0) {
        return buf;
    }
    char my_buf[] = "rwxrwxrwx";
    for (int bit = 1, ind = 0; bit < 512; bit *= 2, ++ind) {
        if (!(perms & bit)) {
            my_buf[8 - ind] = '-';
        }
    }
    if ((perms & S_ISUID) && ((perms & S_IXGRP) || (perms & S_IXOTH))) {
        my_buf[2] = 's';
    }
    if ((perms & S_ISGID) && (perms & S_IXOTH)) {
        my_buf[5] = 's';
    }
    if ((perms & S_ISVTX) && (perms & S_IWOTH) && (perms & S_IXOTH)) {
        my_buf[8] = 't';
    }
    for (int i = 0; i < size - 1; ++i) {
        buf[i] = my_buf[i];
    }
    buf[size - 1] = '\0';
    return buf;
}