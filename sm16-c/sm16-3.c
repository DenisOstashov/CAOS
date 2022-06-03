#include "setjmp.h"
#include "unistd.h"

enum {
    ERROR = 13,
    BUF_SIZE = 10,
};

jmp_buf env[BUF_SIZE];

int ind = 0;

int exceptions[BUF_SIZE];

jmp_buf* try(int exception) {
    exceptions[ind] = exception;
    ++ind;
    return env + ind - 1;
}

void endtry() {
    --ind;
}

void throw(int exception) {
    for (int i = ind - 1; i >= 0; --i) {
        if (exceptions[i] == exception) {
            ind = i;
            longjmp(env[i], exception);
        }
    }
    _exit(ERROR);
}