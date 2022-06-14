#include "sys/types.h"
#include "sys/socket.h"
#include "sys/wait.h"
#include "string.h"
#include "netdb.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "inttypes.h"
#include "unistd.h"
#include "signal.h"

enum {
    LISTEN_LIMIT = 5,
};

volatile sig_atomic_t alive = 1;
volatile sig_atomic_t zombie = 0;

void handler(int signum) {
    if (signum == SIGTERM) {
        alive = 0;
    } else {
        zombie = 1;
    }
}

void destruct(pid_t *children) {
    for (int i = 0; i < LISTEN_LIMIT; ++i) {
        if (children[i] != -1) {
            if (waitpid(children[i], NULL, WNOHANG) != children[i]) {
                kill(children[i], SIGKILL);
                waitpid(children[i], NULL, 0);
            }
            children[i] = -1;
        }
    }
}

void close_children(pid_t *children) {
    for (int i = 0; i < LISTEN_LIMIT; ++i) {
        if (children[i] != -1 && waitpid(children[i], NULL, WNOHANG) == children[i]) {
            children[i] = -1;
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        return 1;
    }
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);
    char *args[argc - 1];
    memcpy(args, argv + 2, (argc - 2) * sizeof(char *));
    args[argc - 2] = NULL;
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *res = NULL;
    int gai_err, one = 1, sock = -1;
    if ((gai_err = getaddrinfo(NULL, argv[1], &hints, &res))) {
        return 1;
    }
    for (struct addrinfo *cur = res; cur; cur = cur->ai_next) {
        if ((sock = socket(cur->ai_family, cur->ai_socktype, 0)) < 0) {
            continue;
        }
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one));
        if (bind(sock, cur->ai_addr, cur->ai_addrlen) < 0 || listen(sock, LISTEN_LIMIT) < 0) {
            close(sock);
            sock = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    if (sock < 0) {
        return 1;
    }
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);
    pid_t children[LISTEN_LIMIT];
    for (int i = 0; i < LISTEN_LIMIT; ++i) {
        children[i] = -1;
    }
    while (alive) {
        int client = accept(sock, (struct sockaddr *) &addr, &addrlen);
        if (client == -1) {
            if (!alive) {
                break;
            } else if (zombie) {
                close_children(children);
                zombie = 0;
            }
            continue;
        }
        if (zombie) {
            close_children(children);
            zombie = 0;
        }
        int ind = 0;
        for (;ind < LISTEN_LIMIT; ++ind) {
            if (children[ind] == -1) {
                break;
            }
        }
        if (ind == LISTEN_LIMIT) {
            break;
        }
        if ((children[ind] = fork()) == 0) {
            close(sock);
            dup2(client, STDIN_FILENO);
            dup2(client, STDOUT_FILENO);
            close(client);
            execvp(args[0], args);
            _exit(0);
        }
        close(client);
    }
    destruct(children);
    close(sock);
}