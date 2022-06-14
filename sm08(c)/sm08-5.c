#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

typedef double (*funcptr_t) (double);

struct FunctionTable {
    const char *str;
    funcptr_t ptr;
};

static const struct FunctionTable table[] = {
        {"sin", &sin},
        {"cos", &cos},
        {"exp", &exp},
        {"log", &log},
        {"tan", &tan},
        {"sqrt", &sqrt}
};

char *gets2(FILE *f) {
    size_t size = 0;
    size_t capacity = 32;
    char *array = (char*) malloc(capacity * sizeof(char));
    if (array == NULL) {
        return NULL;
    }
    int c = fgetc(f);
    while (c == ' ' || c == '\n') {
        c = fgetc(f);
    }
    if (c == EOF) {
        free(array);
        return NULL;
    }
    while (c != EOF && c != ' ' && c != '\n') {
        array[size] = c;
        ++size;
        if (size == capacity) {
            capacity *= 2;
            char *new_array = (char*)realloc(array, capacity * sizeof(char));
            if (new_array == NULL) {
                free(array);
                return NULL;
            }
            array = new_array;
        }
        c = fgetc(f);
    }
    array[size] = '\0';
    return array;
}

funcptr_t get_func(char *name) {
    static const size_t table_size = sizeof(table) / sizeof(struct FunctionTable);
    for (size_t i = 0; i < table_size; ++i) {
        if (strcmp(name, table[i].str) == 0) {
            return table[i].ptr;
        }
    }
    return NULL;
}

int main () {
    char *name = gets2(stdin);
    double value;
    while (name != NULL) {
        if (scanf("%lf", &value) != 1) {
            return 1;
        }
        funcptr_t fptr = get_func(name);
        if (fptr == NULL) {
            printf("%a\n", NAN);
        } else {
            printf("%a\n", fptr(value));
        }
        free(name);
        name = gets2(stdin);
    }
}
