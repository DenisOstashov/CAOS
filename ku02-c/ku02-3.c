#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct data {
    int (*get_rank)(const struct data*);
    char secret[];
};

int comp(const void * l, const void * r) {
    struct data *lp = *((struct data **) l);
    struct data *rp = *((struct data **) r);
    if (lp->get_rank(lp) < rp->get_rank(rp)) {
        return -1;
    } else if (lp->get_rank(lp) > rp->get_rank(rp)) {
        return 1;
    }
    return 0;
}

void rank_sort(struct data* elements[], size_t size) {
    qsort(elements, size, sizeof(struct data*), &comp);
}