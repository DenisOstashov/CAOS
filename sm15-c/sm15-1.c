#include "sys/types.h"
#include "sys/socket.h"
#include "netdb.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "inttypes.h"

enum {
    BUF_SIZE = 1024,
};

int main() {
    char host_buf[BUF_SIZE], service_buf[BUF_SIZE];
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    while (scanf("%s %s", host_buf, service_buf) == 2) {
        struct addrinfo *res = NULL;
        int gai_err;
        if ((gai_err = getaddrinfo(host_buf, service_buf, &hints, &res))) {
            printf("%s\n", gai_strerror(gai_err));
            continue;
        }
        struct sockaddr_in *min = (struct sockaddr_in *) res->ai_addr;
        for (struct addrinfo *cur = res->ai_next; cur; cur = cur->ai_next) {
            if (ntohl(((struct sockaddr_in *)cur->ai_addr)->sin_addr.s_addr)
                    < ntohl(min->sin_addr.s_addr)) {
                min = (struct sockaddr_in *) cur->ai_addr;
            }
        }
        printf("%s:%" PRIu16 "\n", inet_ntoa(min->sin_addr), ntohs(min->sin_port));
        freeaddrinfo(res);
    }
}