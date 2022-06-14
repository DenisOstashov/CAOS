#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

enum {
    RW_ALL = 0666,
};

int exec_cmd(char *filename, char *cmd, int flag) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        if (flag) {
            int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, RW_ALL);
            if (fd < 0) {
                return 1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execlp(cmd, cmd, NULL);
        _exit(1);
    }
    if (pid == -1) {
        return 1;
    }
    int st;
    waitpid(pid, &st, 0);
    return !WIFEXITED(st) || WEXITSTATUS(st) != 0;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        return 1;
    }
    return exec_cmd(NULL, argv[1], 0) == 1 && exec_cmd(argv[3], argv[2], 1) == 1;
}