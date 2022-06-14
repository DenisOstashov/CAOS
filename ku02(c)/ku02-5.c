#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct dag_node {
    struct dag_node *left, *right;
    void *data;
};

void mark(struct dag_node *ptr, void *secret) {
    if (ptr == NULL) {
        return;
    }
    if (ptr->left != NULL && ptr->left->data != secret) {
        mark(ptr->left, secret);
    } else {
        ptr->left = NULL;
    }
    if (ptr->right != NULL && ptr->right->data != secret) {
        mark(ptr->right, secret);
    } else {
        ptr->right = NULL;
    }
    ptr->data = secret;
}

void in_order(struct dag_node *ptr) {
    if (ptr == NULL) {
        return;
    }
    in_order(ptr->left);
    in_order(ptr->right);
    free(ptr);
}

void free_dag(struct dag_node *root) {
    if (root == NULL) {
        return;
    }
    void *secret = malloc(sizeof(size_t));
    mark(root, secret);
    free(secret);
    in_order(root);
}