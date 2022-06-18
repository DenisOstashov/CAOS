#include "unistd.h"
#include "limits.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"

uintptr_t get_begin(char *maps_path, const char * lib) {
    enum {
        LINE_SIZE = 1024,
        HEX_BASE = 16,
    };
    FILE *maps_file = fopen(maps_path, "r");
    char new_line[LINE_SIZE];
    uintptr_t begin = 0;
    while (fgets(new_line, LINE_SIZE, maps_file)) {
        if (strstr(new_line, lib)) {
            begin = strtol(strtok(new_line, "-"), NULL, HEX_BASE);
            break;
        }
    }
    fclose(maps_file);
    return begin;
}

typedef void caos_func_ptr(int);

int main() {
    int interactor_pid, arg;
    if (read(STDIN_FILENO, &interactor_pid, sizeof(interactor_pid)) == -1) {
        return 1;
    }
    char interactor_path[PATH_MAX];
    snprintf(interactor_path, PATH_MAX, "/proc/%d/maps", interactor_pid);
    uintptr_t interactor_ptr, self_begin = get_begin("/proc/self/maps", "libroutines.so");
    uintptr_t interactor_begin = get_begin(interactor_path, "libroutines.so");
    while (read(STDIN_FILENO, &interactor_ptr, sizeof(uintptr_t)) == sizeof(uintptr_t) &&
           read(STDIN_FILENO, &arg, sizeof(arg)) == sizeof(arg)) {
        uintptr_t real_ptr = self_begin + (interactor_ptr - interactor_begin);
        ((caos_func_ptr *)(real_ptr))(arg);
    }
}