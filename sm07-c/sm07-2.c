#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    int count;
    struct Node* next;
    struct Node* prev;
};

int main() {
    int value;
    if (scanf("%d", &value) != 1) {
        return 0;
    }
    struct Node *begin = (struct Node*) malloc(sizeof(struct Node));
    if (begin == NULL) {
        return 1;
    }
    struct Node *end = (struct Node*) malloc(sizeof(struct Node));
    if (end == NULL) {
        free(begin);
        return 1;
    }
    begin->value = value;
    begin->count = 1;
    begin->next = end;
    end->prev = begin;
    while (scanf("%d", &value) == 1) {
        int found = 0;
        for (struct Node *ptr = begin; ptr != end; ptr = ptr->next) {
            if (ptr->value == value) {
                ++ptr->count;
                if (ptr != begin) {
                    ptr->next->prev = ptr->prev;
                    ptr->prev->next = ptr->next;
                    begin->prev = ptr;
                    ptr->next = begin;
                    begin = ptr;
                }
                found = 1;
                break;
            }
        }
        if (found == 0) {
            struct Node *new_begin = (struct Node*) malloc(sizeof(struct Node));
            if (new_begin == NULL) {
                while (begin != end) {
                    new_begin = begin->next;
                    free(begin);
                    begin = new_begin;
                }
                free(end);
                return 1;
            }
            new_begin->value = value;
            new_begin->count = 1;
            new_begin->next = begin;
            begin->prev = new_begin;
            begin = new_begin;
        }
    }
    while (end != begin) {
        struct Node *new_end = end->prev;
        free(end);
        printf("%d %d\n", new_end->value, new_end->count);
        end = new_end;
    }
    free(begin);
}