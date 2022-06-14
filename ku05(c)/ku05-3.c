#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

enum {
    RW_ALL = 0666,
};

int exec_cmd(char *filename, char *cmd) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        int fd = open(filename, O_RDWR | O_CREAT | O_APPEND, RW_ALL);
        if (fd < 0) {
            return 1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
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
    return exec_cmd(argv[3], argv[1]) == 1 && exec_cmd(argv[3], argv[2]) == 1;
}