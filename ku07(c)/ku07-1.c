#include "sys/types.h"
#include "sys/socket.h"
#include "netdb.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "inttypes.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include "stdbool.h"

struct addresss_ {
    unsigned f;
    unsigned s;
    unsigned t;
    unsigned fo;
} mask__, subnet__, address__;

bool is_ipv4_subnet(const char* mask, const char* subnet_address, const char* address) {
    sscanf(mask, "%u.%u.%u.%u",  &mask__.f, &mask__.s, &mask__.t, &mask__.fo);
    sscanf(subnet_address, "%u.%u.%u.%u",  &subnet__.f, &subnet__.s, &subnet__.t, &subnet__.fo);
    sscanf(address, "%u.%u.%u.%u",  &address__.f, &address__.s, &address__.t, &address__.fo);
    /*printf("%u.%u.%u.%u\n", mask__.f, mask__.s, mask__.t, mask__.fo);
    printf("%u.%u.%u.%u\n", subnet__.f, subnet__.s, subnet__.t, subnet__.fo);
    printf("%u.%u.%u.%u\n", address__.f, address__.s, address__.t, address__.fo);*/
    return (mask__.f & address__.f) == subnet__.f && (mask__.s & address__.s) == subnet__.s &&
           (mask__.t & address__.t) == subnet__.t && (mask__.fo & address__.fo) == subnet__.fo;
}
/*

int main() {
    char buf[1000], buf1[1000], buf2[1000];
    while (scanf("%s %s %s", buf, buf1, buf2) == 3) {
        printf("%d\n", is_ipv4_subnet(buf, buf1, buf2));
    }
}*/