#include <stdio.h>
#include <stdlib.h>

char *getline2(FILE *f) {
    size_t size = 1;
    size_t capacity = 256;
    char *array = (char*) malloc(capacity * sizeof(char));
    if (array == NULL) {
        return NULL;
    }
    int c = fgetc(f);
    if (c == EOF) {
        free(array);
        return NULL;
    }
    array[0] = c;
    while (array[size - 1] != '\n') {
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
        if (c == EOF) {
            break;
        }
        array[size] = c;
        ++size;
    }
    if (size == capacity) {
        char *new_array = (char*)realloc(array, (capacity + 1) * sizeof(char));
        if (new_array == NULL) {
            free(array);
            return NULL;
        }
        array = new_array;
    }
    array[size] = '\0';
    return array;
}
