#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

enum {
    ERROR = 127,
    BLOCK_SIZE = 2048,
};

int interact(const char* cmd, const char* input, char* outbuf) {
    int pipes[2][2];
    pipe(pipes[0]);
    pipe(pipes[1]);
    pid_t pid;
    if ((pid = fork()) == 0) {
        close(pipes[0][1]);
        close(pipes[1][0]);
        dup2(pipes[0][0], STDIN_FILENO);
        dup2(pipes[1][1], STDOUT_FILENO);
        close(pipes[0][0]);
        close(pipes[1][1]);
        execlp(cmd, cmd, NULL);
        _Exit(127);
    } else if (pid == -1) {
        close(pipes[0][1]);
        close(pipes[1][0]);
        close(pipes[0][0]);
        close(pipes[1][1]);
        return ERROR;
    }
    size_t len = strlen(input);
    close(pipes[0][0]);
    close(pipes[1][1]);
    if (write(pipes[0][1], input, len)) {}
    close(pipes[0][1]);
    ssize_t read_result;
    while ((read_result = read(pipes[1][0], outbuf, BLOCK_SIZE)) > 0) {
        outbuf += read_result;
    }
    outbuf[0] = '\0';
    close(pipes[1][0]);
    int ans;
    waitpid(pid, &ans, 0);
    if (WIFEXITED(ans)) {
        return WEXITSTATUS(ans);
    } else {
        return ERROR;
    }
}