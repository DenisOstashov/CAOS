#include <stdlib.h>
#include <stdio.h>

enum {
    MULT = 1103515245,
    INC = 12345,
    MOD = 2147483648,
};

struct RandomOperations;

typedef struct RandomGenerator {
    unsigned int value;
    struct RandomOperations* ops;
} RandomGenerator;

typedef struct RandomOperations {
    void (*destroy) (RandomGenerator *gen);
    int (*next) (RandomGenerator *gen);
} RandomOperations;

void destroy_impl(RandomGenerator *gen) {
    if (gen == NULL) {
        return;
    }
    free(gen);
}

int next_impl(RandomGenerator *gen) {
    gen->value = (gen->value * (unsigned) MULT + (unsigned) INC) % (unsigned) MOD;
    return gen->value;
}

RandomOperations Operations = {.destroy = &destroy_impl, .next = &next_impl};

RandomGenerator *random_create(int seed) {
    RandomGenerator* gen = (RandomGenerator*) malloc(sizeof(RandomGenerator));
    if (gen == NULL) {
        return NULL;
    }
    gen->ops = &Operations;
    gen->value = seed;
    return gen;
}