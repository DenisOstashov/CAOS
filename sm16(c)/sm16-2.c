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
#include "stdio.h"

enum {
    BUF_SIZE = 1024,
};

int sock = -1;

void handler(int signum) {
    close(sock);
    _exit(0);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        return 1;
    }
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *res = NULL;
    int gai_err;
    if ((gai_err = getaddrinfo(NULL, argv[1], &hints, &res))) {
        return 1;
    }
    for (struct addrinfo *cur = res; cur; cur = cur->ai_next) {
        if ((sock = socket(cur->ai_family, cur->ai_socktype, 0)) < 0) {
            continue;
        }
        if (bind(sock, cur->ai_addr, cur->ai_addrlen) < 0) {
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
    char buf[BUF_SIZE + 1];
    while (1) {
        ssize_t ret = recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&addr, &addrlen);
        buf[ret] = '\0';
        char host[100], port[10];
        getnameinfo((struct sockaddr *)&addr, addrlen, host, sizeof(host),
                    port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV);
        printf("[%s:%s] %s\n", host, port, buf);
        fflush(stdout);
    }
    close(sock);
}