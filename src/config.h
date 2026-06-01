#ifndef _CONFIG_
#define _CONFIG_

#include <stdint.h>
#include <stddef.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PROTOCOL_TCP    SOCK_STREAM
#define PROTOCOL_UDP    SOCK_DGRAM
#define IPV4            AF_INET
#define IPV6            AF_INET6

enum test_case_type
{
    BIN,
    HEX,
    TEXT
};

struct target
{
    int protocol;
    int family;
    char ip_address[INET6_ADDRSTRLEN];
    uint16_t port; 
};

struct test_case
{
    enum test_case_type case_t;
    void *test_data; 
    void *expected_output;
    size_t test_data_size; 
    size_t expected_size;
};

struct test_node
{
    struct test_case *test_cases;
    size_t test_cases_capacity; 
    size_t tests_count; 
};

struct test_file 
{
    struct target target;
    struct test_node *nodes; 
    size_t nodes_capacity;
    size_t nodes_count; 
};

int parse_yaml_files(char **files, size_t files_num, struct test_file **dest);

#endif