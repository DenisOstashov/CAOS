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

int main(int argc, char **argv) {
    if (argc < 2) {
        return 1;
    }
    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *res = NULL;
    int gai_err, one = 1, sock = -1;
    int64_t sum = 0;
    int32_t val;
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
    while (1) {
        int client = accept(sock, (struct sockaddr *) &addr, &addrlen);
        if (read(client, &val, sizeof(val)) != sizeof(val)) {
            close(client);
            break;
        }
        val = (int32_t) ntohl(val);
        sum += val;
        close(client);
        if (val == 0) {
            break;
        }
    }
    close(sock);
    printf("%" PRId64 "\n", sum);
}