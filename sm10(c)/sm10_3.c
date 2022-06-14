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

int main(int argc, char **argv) {
    DIR *directory = opendir(argv[1]);
    if (directory == NULL) {
        puts("0");
        return 0;
    }
    char full_path[PATH_MAX];
    size_t len = snprintf(full_path, PATH_MAX, "%s", argv[1]);
    char *start = full_path + len;
    size_t limit = PATH_MAX - len;
    uint64_t result = 0;
    struct dirent *dire;
    struct stat st;
    uid_t uid = getuid();
    while ((dire = readdir(directory)) != NULL) {
        snprintf(start, limit, "/%s", dire->d_name);
        if (stat(full_path, &st) != -1 && S_ISREG(st.st_mode)
            && uid == st.st_uid && isupper(dire->d_name[0])) {
            result += st.st_size;
        }
    }
    printf("%" PRIu64 "\n", result);
    closedir(directory);
}