#include "sys/types.h"
#include "sys/socket.h"
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
    BUF_SIZE = 1024,
};

volatile sig_atomic_t socket_closed = 0;

void handler(int signum) {
    socket_closed = 1;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        return 1;
    }
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGPIPE, &sa, NULL);
    int sock = -1;
    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *res = NULL;
    int gai_err;
    int64_t num;
    if ((gai_err = getaddrinfo(argv[1], argv[2], &hints, &res))) {
        return 1;
    }
    for (struct addrinfo *cur = res; cur; cur = cur->ai_next) {
        if ((sock = socket(cur->ai_family, cur->ai_socktype, 0)) < 0 ||
            connect(sock, cur->ai_addr, cur->ai_addrlen) < 0) {
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    if (sock < 0) {
        return 1;
    }
    FILE *in = fdopen(sock, "r");
    int dupped = dup(sock);
    FILE *out = fdopen(dupped, "w");
    if (fprintf(out, "%s\n", argv[3]) < 0 || socket_closed) {
        EXIT(in, out, sock, dupped);
    }
    fflush(out);
    if (fscanf(in, "%" SCNd64 , &num) != 1 || socket_closed) {
        EXIT(in, out, sock, dupped);
    }
    for (int64_t i = 0; i <= num; ++i) {
        if (fprintf(out, "%" PRId64 "\n", i) < 0 || socket_closed) {
            EXIT(in, out, sock, dupped);
        }
        fflush(out);
    }
    if (fscanf(in, "%" SCNd64 , &num) != 1 || socket_closed) {
        EXIT(in, out, sock, dupped);
    }
    printf("%" PRId64"\n", num);
    fflush(stdout);
    EXIT(in, out, sock, dupped);
}