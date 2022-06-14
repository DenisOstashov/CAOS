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

#define EXIT(IN, OUT, FD1, FD2) \
    do {                        \
        fclose(IN);             \
        fclose(OUT);            \
        close(FD1);             \
        close(FD2);             \
        return 0;               \
       } while (0)

enum {
    LISTEN_LIMIT = 64,
};

volatile sig_atomic_t alive = 1;
volatile sig_atomic_t zombie = 0;
volatile sig_atomic_t closed = 0;

void handler(int signum) {
    if (signum == SIGTERM) {
        alive = 0;
    } else if (signum == SIGCHLD) {
        zombie = 1;
    } else if (signum == SIGPIPE) {
        closed = 1;
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
    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
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
    int32_t serial = 1;
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
            sigaction(SIGPIPE, &sa, NULL);
            FILE *in = fdopen(client, "r");
            int dupped = dup(client);
            FILE *out = fdopen(dupped, "w");
            if (fprintf(out, "%s\r\n", argv[2]) < 0 || closed) {
                EXIT(in, out, client, dupped);
            }
            fflush(out);
            if (fprintf(out, "%" PRId32"\r\n", serial) < 0 || closed) {
                EXIT(in, out, client, dupped);
            }
            fflush(out);
            int32_t max, num;
            if (fscanf(in, "%" SCNd32 "", &max) != 1 || closed) {
                EXIT(in, out, client, dupped);
            }
            if (fprintf(out, "%" PRId32 "\r\n", max) < 0 || closed) {
                EXIT(in, out, client, dupped);
            }
            fflush(out);
            while (fscanf(in, "%" SCNd32 "", &num) == 1 && !closed) {
                if (num > max || num > INT32_MAX - serial) {
                    fprintf(out, "-1\r\n");
                    fflush(out);
                    EXIT(in, out, client, dupped);
                }
                if (fprintf(out, "%" PRId32"\r\n", num + serial) < 0 || closed) {
                    EXIT(in, out, client, dupped);
                }
                fflush(out);
            }
            EXIT(in, out, client, dupped);
        }
        ++serial;
        close(client);
    }
    destruct(children);
    close(sock);
}