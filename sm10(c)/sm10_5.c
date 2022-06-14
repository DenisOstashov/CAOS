#include <sys/types.h>
#include <dirent.h>
#include <strings.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <inttypes.h>
#include <dlfcn.h>

typedef double (*funcptr_t) (double);

int main(int argc, char **argv) {
    void *dl = dlopen("libm.so.6", RTLD_LAZY);
    if (dl == NULL) {
        return 1;
    }
    void *fptr = dlsym(dl, argv[1]);
    if (fptr == NULL) {
        return 1;
    }
    funcptr_t f = (funcptr_t)fptr;
    double value;
    while (scanf("%lf", &value) == 1) {
        printf("%.10g\n", f(value));
    }
    if (dlclose(dl)) {
        return 1;
    }
    return 0;
}
