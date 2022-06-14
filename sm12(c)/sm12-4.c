#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"
#include "stdlib.h"
#include "inttypes.h"
#include "string.h"
#include "limits.h"

int main(int argc, char** argv) {
    char full_path[PATH_MAX] = "/tmp/";
    char *path;
    int64_t pid = getpid(), start = strlen(full_path);
    if ((path = getenv("XDG_RUNTIME_DIR")) != NULL || (path = getenv("TMPDIR")) != NULL) {
        start = snprintf(full_path, PATH_MAX, "%s/", path);
    }
    snprintf(full_path + start, PATH_MAX - start, "%" PRId64 ".py", pid);
    int fd = open(full_path, O_RDWR | O_TRUNC | O_CREAT | O_EXCL, S_IRWXU);
    FILE *file = fdopen(fd, "r+");
    fprintf(file, "#!/bin/python3\nprint(%s", argv[1]);
    for (int i = 2; i < argc; ++i) {
        fprintf(file, " * %s", argv[i]);
    }
    fprintf(file, ")\nimport os\nos.remove(__file__)\n");
    fclose(file);
    close(fd);
    execl(full_path, full_path, NULL);
}