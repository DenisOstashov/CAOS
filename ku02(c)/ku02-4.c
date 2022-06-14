#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int vercmp(const char* v1, const char* v2) {
    while (*v1 != '\0' && *v2 != '\0') {
        while (*v1 != '\0' && (*v1 < '0' || *v1 > '9')) {
            ++v1;
        }
        if (*v1 == '0') {
            while (*v1 == '0') {
                ++v1;
            }
            if (*v1 < '0' || *v1 > '9') {
                --v1;
            }
        }
        while (*v2 != '\0' && (*v2 < '0' || *v2 > '9')) {
            ++v2;
        }
        if (*v2 == '0') {
            while (*v2 == '0') {
                ++v2;
            }
            if (*v2 < '0' || *v2 > '9') {
                --v2;
            }
        }
        if (*v1 == '\0' || *v2 == '\0') {
            if (*v1 == '\0' && *v2 == '\0') {
                return 0;
            } else if (*v1 == '\0') {
                return -1;
            } else {
                return 1;
            }
        }
        size_t len_1 = 0;
        size_t len_2 = 0;
        const char* it_1 = v1;
        const char* it_2 = v2;
        while (*it_1 >= '0' && *it_1 <= '9') {
            ++it_1;
            ++len_1;
        }
        while (*it_2 >= '0' && *it_2 <= '9') {
            ++it_2;
            ++len_2;
        }
        if (len_1 < len_2) {
            return -1;
        } else if (len_1 > len_2) {
            return 1;
        }
        while (v1 != it_1) {
            if (*v1 < *v2) {
                return -1;
            } else if (*v1 > *v2) {
                return 1;
            }
            ++v1;
            ++v2;
        }
    }
    while (*v1 != '\0' && (*v1 < '0' || *v1 > '9')) {
        ++v1;
    }
    while (*v2 != '\0' && (*v2 < '0' || *v2 > '9')) {
        ++v2;
    }
    if (*v1 == '\0' && *v2 == '\0') {
        return 0;
    } else if (*v1 == '\0') {
        return -1;
    } else {
        return 1;
    }
}