#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"
#include "sys/wait.h"

int main(int argc, char** argv) {
    //"Процесс-родитель всегда завершается с кодом 0" -> ошибки обрабатывать не буду
    pid_t pid;
    if ((pid = fork()) == 0) {
        int input_fd = open(argv[2], O_RDONLY);
        int output_fd = open(argv[3], O_RDWR | O_CREAT | O_TRUNC,
                             S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);
        close(input_fd);
        close(output_fd);
        execlp(argv[1], argv[1], NULL);
        return 1;
    } else {
        wait(NULL);
        return 0;
    }
}