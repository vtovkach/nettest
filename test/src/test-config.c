#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"

#define TEST_FILES_NUM 4

// Free allocations later

void assert_parse_yaml_files(struct iso_test_node *nodes, int nodes_num)
{
    struct iso_test_node cur_node = nodes[0];
    assert(cur_node.target.protocol == PROTOCOL_UDP);
    assert(cur_node.target.family == IPV4);
    assert(strcmp(cur_node.target.ip_address, "127.0.0.1") == 0);
    assert(cur_node.target.port == 10000);

    cur_node = nodes[1];
    assert(cur_node.target.protocol == PROTOCOL_TCP);
    assert(cur_node.target.family == IPV4);
    assert(strcmp(cur_node.target.ip_address, "127.0.0.1") == 0);
    assert(cur_node.target.port == 10001);

    cur_node = nodes[2];
    assert(cur_node.target.protocol == PROTOCOL_UDP);
    assert(cur_node.target.family == IPV6);
    assert(strcmp(cur_node.target.ip_address, "::1") == 0);
    assert(cur_node.target.port == 10003);

    cur_node = nodes[3];
    assert(cur_node.target.protocol == PROTOCOL_TCP);
    assert(cur_node.target.family == IPV6);
    assert(strcmp(cur_node.target.ip_address, "::2") == 0);
    assert(cur_node.target.port == 5678);
}

void test_config(void)
{
    // Later create a separate process to ensure test isolation 
    char *files[TEST_FILES_NUM];
    files[0] = "test-yaml/test1.yaml";
    files[1] = "test-yaml/test2.yaml";
    files[2] = "test-yaml/test3.yaml";
    files[3] = "test-yaml/test4.yaml";

    struct iso_test_node *nodes = NULL;
    parse_yaml_files(files, TEST_FILES_NUM, &nodes);
    assert_parse_yaml_files(nodes, TEST_FILES_NUM);
}