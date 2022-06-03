#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node* left;
    struct Node* right;
};

void insert(struct Node *ptr, int value) {
    if (value == ptr->value) {
        return;
    }
    if (value < ptr->value) {
        if (ptr->left == NULL) {
            ptr->left = (struct Node*) malloc(sizeof(struct Node));
            ptr->left->value = value;
            ptr->left->right = NULL;
            ptr->left->left = NULL;
        } else {
            insert(ptr->left, value);
        }
    } else if (ptr->right == NULL) {
        ptr->right = (struct Node*) malloc(sizeof(struct Node));
        ptr->right->value = value;
        ptr->right->right = NULL;
        ptr->right->left = NULL;
    } else {
        insert(ptr->right, value);
    }
}

void print(struct Node *ptr) {
    if (ptr->right != NULL) {
        print(ptr->right);
        free(ptr->right);
    }
    printf("%d ", ptr->value);
    if (ptr->left != NULL) {
        print(ptr->left);
        free(ptr->left);
    }
}

int main() {
    int value;
    struct Node *root = NULL;
    while (scanf("%d", &value) == 1) {
        if (value == 0) {
            if (root != NULL) {
                print(root);
                free(root);
                root = NULL;
            }
            fputs("0 ", stdout);
        } else {
            if (root == NULL) {
                root = (struct Node*) malloc(sizeof(struct Node));
                if (root == NULL) {
                    return 1;
                }
                root->value = value;
                root->left = NULL;
                root->right = NULL;
            } else {
                insert(root, value);
            }
        }
    }
    if (root != NULL) {
        print(root);
        free(root);
        fputs("0 ", stdout);
    }
    puts("");
}
