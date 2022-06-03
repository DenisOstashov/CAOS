#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef void (*finalizer_t)(void *ptr, size_t size);

void gc_init(char **argv);
void *gc_malloc(size_t size, finalizer_t finalizer);
void gc_collect();


struct allocation {
    void *ptr; // указатель на область памяти
    size_t size; // размер
    finalizer_t finalizer; // функция, которую следует вызвать перед free
    bool alive; // достижимость
};

bool points_to(void *ptr, struct allocation *A) {
    uintptr_t uptr = (uintptr_t)ptr, aptr = (uintptr_t)A->ptr;
    return (uptr >= aptr) && (uptr - aptr <= A->size);
}

void **stack_bottom;

struct Node {
    struct Node *next;
    struct Node *prev;
    struct allocation *value;
} start;

struct Node finish;

void gc_init(char **argv) {
    stack_bottom = (void**)argv;
    start.next = &finish;
    finish.prev = &start;
}

void push(struct allocation *new_allocation) {
    struct Node *new_node = (struct Node*) malloc(sizeof(struct Node));
    if (new_node == NULL) {
        return;//?
    }
    new_node->prev = finish.prev;
    new_node->next = &finish;
    finish.prev->next = new_node;
    finish.prev = new_node;
    new_node->value = new_allocation;
}

void *gc_malloc(size_t size, finalizer_t finalizer) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        return NULL;//?
    }
    struct allocation *new_allocation = (struct allocation*) malloc(sizeof(struct allocation));
    if (new_allocation == NULL) {
        free(ptr);
        return NULL;//?
    }
    new_allocation->ptr = ptr;
    new_allocation->size = size;
    new_allocation->finalizer = finalizer;
    push(new_allocation);
    if (finish.prev->value != new_allocation) {
        free(ptr);
        free(new_allocation);
        return NULL;
    }
    return ptr;
}

struct allocation *find(void *ptr) {
    for (struct Node *it = start.next; it != &finish; it = it->next) {
        if (points_to(ptr, it->value)) {
            return it->value;
        }
    }
    return NULL;
}

void markAl(struct allocation *Al) {
    void **ptr = (void**)Al->ptr;
    size_t size = Al->size;
    while (size >= sizeof(void*)) {
        struct allocation *A = find(*ptr);
        if (A != NULL && !A->alive) {
            A->alive = true;
            markAl(A);
        }
	++ptr;
	size -= sizeof(void*);
    }
}

void mark(void **begin, void **end) {
    while (begin < end) {
        struct allocation *A = find(*begin);
        if (A != NULL && !A->alive) {
            A->alive = true;
            markAl(A);
        }
        ++begin;
    }
}

void gc_collect_impl(uintptr_t stack_top) {
    for (struct Node *ptr = start.next; ptr != &finish; ptr = ptr->next) {
        ptr->value->alive = false;
    }
    mark((void **)stack_top, stack_bottom);
    for (struct Node *ptr = start.next; ptr != &finish;) {
        if (!ptr->value->alive) {
            if (ptr->value->finalizer != NULL) {
                ptr->value->finalizer(ptr->value->ptr, ptr->value->size);
            }
            free(ptr->value->ptr);
            free(ptr->value);
            ptr->prev->next = ptr->next;
            ptr->next->prev = ptr->prev;
            struct Node *tmp = ptr->next;
            free(ptr);
            ptr = tmp;
        } else {
            ptr = ptr->next;
        }
    }
}
