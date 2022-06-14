#include <stdlib.h>
#include <stdio.h>

struct Node {
    int value;
    struct Node* prev;
};

int main() {
    struct Node* head = NULL;
    int value;
    while (scanf("%d", &value) == 1) {
        struct Node* new_head = (struct Node*) malloc(sizeof(struct Node));
        if (new_head == NULL) {
            return 1;
        }
        new_head->value = value;
        new_head->prev = head;
        head = new_head;
    }
    while (head != NULL) {
        printf("%d\n", head->value);
        struct Node* new_head = head->prev;
        free(head);
        head = new_head;
    }
    return 0;
}
