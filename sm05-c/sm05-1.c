void normalize_path(char *buf) {
    char *left = buf, *right = buf;
    int skip = 0;
    while (*right) {
        if (*right == '/' && skip) {
            ++right;
            continue;
        }
        *left = *right;
        skip = *right == '/';
        ++left;
        ++right;
    }
    *left = *right;
}