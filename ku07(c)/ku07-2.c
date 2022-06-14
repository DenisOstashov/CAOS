#include "sys/types.h"
#include "sys/socket.h"
#include "netdb.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "inttypes.h"
#include "unistd.h"
#include "signal.h"

enum {
    LISTEN_LIMIT = 5,
};

int client = -1, sock = -1;

volatile sig_atomic_t alive = 1;
volatile sig_atomic_t closed = 0;

void handler(int signum) {
    if (signum == SIGTERM) {
        if (client != -1) {
            close(client);
        }
        close(sock);
        _exit(0);
        alive = 0;
    } else if (signum == SIGPIPE) {
        closed = 1;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        return 1;
    }
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGPIPE, &sa, NULL);
    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *res = NULL;
    int gai_err, one = 1;
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
    int empty_num = 1;
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);
    char buf[1024];
    while (alive) {
        client = accept(sock, (struct sockaddr *) &addr, &addrlen);
        if (client == -1) {
            if (!alive) {
                break;
            }
            continue;
        }
        closed = 0;
        int empty = 1;
        ssize_t res;
        while ((res = read(client, buf, 1024)) > 0) {
            empty = 0;
            char *ptr = buf;
            ssize_t left = res, written;
            while (!closed && left && (written = write(client, ptr, left)) > 0) {
                left -= written;
                ptr += written;
            }
            if (left > 0 || closed) {
                break;
            }
        }
        if (empty && !closed) {
            FILE * out = fdopen(client, "w");
            if (out) {
                fprintf(out, "Empty message #%d\n", empty_num);
                fflush(out);
                fclose(out);
            }
            ++empty_num;
        }
        close(client);
        client = -1;
    }
    close(sock);
}