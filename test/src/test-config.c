#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"

#define TEST_FILES_NUM 4

// Free allocations later

void assert_parse_yaml_files(struct test_file *t_file, int nodes_capacity)
{
    (void)nodes_capacity;
    struct test_file cur_node = t_file[0];

   /* Assert target */
    assert(cur_node.target.protocol == PROTOCOL_UDP);
    assert(cur_node.target.family == IPV4);
    assert(strcmp(cur_node.target.ip_address, "127.0.0.1") == 0);
    assert(cur_node.target.port == 10000);
    /* Assert test cases */
    assert(strcmp((char *)cur_node.nodes[0].test_cases[0].test_data, "Hello World") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[0].expected_output, "Yo!") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[1].test_data, "Hello! How are you?") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[1].expected_output, "Hi. I am fine!") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].test_data, "Good bye!") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].expected_output, "Bye!") == 0);

    cur_node = t_file[1];
    /* Assert target */
    assert(cur_node.target.protocol == PROTOCOL_TCP);
    assert(cur_node.target.family == IPV4);
    assert(strcmp(cur_node.target.ip_address, "127.0.0.1") == 0);
    assert(cur_node.target.port == 10001);
    /* Assert test cases */
    assert(strcmp((char *)cur_node.nodes[0].test_cases[0].test_data, "ping") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[0].expected_output, "pong") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[1].test_data, "test") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[1].expected_output, "ok") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].test_data, "bye") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].expected_output, "goodbye") == 0);


    cur_node = t_file[2];
    /* Assert target */
    assert(cur_node.target.protocol == PROTOCOL_UDP);
    assert(cur_node.target.family == IPV6);
    assert(strcmp(cur_node.target.ip_address, "::1") == 0);
    assert(cur_node.target.port == 10003);
    /* Assert test cases */
    assert(strcmp((char *)cur_node.nodes[0].test_cases[0].test_data, "connect") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[0].expected_output, "connected") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[1].test_data, "login user") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[1].expected_output, "login ok") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[2].test_data, "quit") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[2].expected_output, "disconnected") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].test_data, "health") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].expected_output, "healthy") == 0);

    cur_node = t_file[3];
    /* Assert target */
    assert(cur_node.target.protocol == PROTOCOL_TCP);
    assert(cur_node.target.family == IPV6);
    assert(strcmp(cur_node.target.ip_address, "::2") == 0);
    assert(cur_node.target.port == 5678);
    /* Assert test cases */
    assert(strcmp((char *)cur_node.nodes[0].test_cases[0].test_data, "connect") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[0].expected_output, "connected") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[1].test_data, "login user") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[1].expected_output, "login ok") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[2].test_data, "quit") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[2].expected_output, "disconnected") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].test_data, "health") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].expected_output, "healthy") == 0);
}

void test_config(void)
{
    // Later create a separate process to ensure test isolation 
    char *files[TEST_FILES_NUM];
    files[0] = "test-yaml/test1.yaml";
    files[1] = "test-yaml/test2.yaml";
    files[2] = "test-yaml/test3.yaml";
    files[3] = "test-yaml/test4.yaml";

    struct test_file *t_file = NULL;
    parse_yaml_files(files, TEST_FILES_NUM, &t_file);
    assert_parse_yaml_files(t_file, TEST_FILES_NUM);
}