#ifndef _TARGET_
#define _TARGET_

#include <stdint.h>
#include <netinet/in.h>

#define PROTOCOL_TCP    1
#define PROTOCOL_UDP    2
#define IPV4            3
#define IPV6            4

struct target_t
{
    uint8_t protocol;
    uint8_t family;
    char ip_address[INET6_ADDRSTRLEN];
    uint16_t port; 
};

#endif