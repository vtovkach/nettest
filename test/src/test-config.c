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

    assert(t_file->nodes_count == 2);

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

    assert(t_file->nodes_count == 2);

    cur_node = t_file[2];
    /* Assert target */
    assert(cur_node.target.protocol == PROTOCOL_UDP);
    assert(cur_node.target.family == IPV4);
    assert(strcmp(cur_node.target.ip_address, "127.0.0.1") == 0);
    assert(cur_node.target.port == 10000);

    /* Assert node 0 */
    assert(strcmp((char *)cur_node.nodes[0].test_cases[0].test_data, "Hello World") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[0].expected_output, "Yo!") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[1].test_data, "Hello! How are you?") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[1].expected_output, "Hi. I am fine!") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[2].test_data, "[in] gg") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[2].expected_output, "[out] gg") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[3].test_data, "Good bye!") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[3].expected_output, "Bye!") == 0);

    /* Assert node 1 */
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].test_data, "Good bye!") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].expected_output, "Bye!") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[1].test_data, "ping") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[1].expected_output, "pong") == 0);

    /* Assert node 2 */
    assert(strcmp((char *)cur_node.nodes[2].test_cases[0].test_data, "connect") == 0);
    assert(strcmp((char *)cur_node.nodes[2].test_cases[0].expected_output, "connected") == 0);
    assert(strcmp((char *)cur_node.nodes[2].test_cases[1].test_data, "login user") == 0);
    assert(strcmp((char *)cur_node.nodes[2].test_cases[1].expected_output, "login ok") == 0);
    assert(strcmp((char *)cur_node.nodes[2].test_cases[2].test_data, "send packet") == 0);
    assert(strcmp((char *)cur_node.nodes[2].test_cases[2].expected_output, "packet received") == 0);
    assert(strcmp((char *)cur_node.nodes[2].test_cases[3].test_data, "quit") == 0);
    assert(strcmp((char *)cur_node.nodes[2].test_cases[3].expected_output, "disconnected") == 0);

    /* Assert node 3 */
    assert(strcmp((char *)cur_node.nodes[3].test_cases[0].test_data, "health") == 0);
    assert(strcmp((char *)cur_node.nodes[3].test_cases[0].expected_output, "healthy") == 0);
    assert(strcmp((char *)cur_node.nodes[3].test_cases[1].test_data, "status") == 0);
    assert(strcmp((char *)cur_node.nodes[3].test_cases[1].expected_output, "ok") == 0);
    assert(strcmp((char *)cur_node.nodes[3].test_cases[2].test_data, "reset") == 0);
    assert(strcmp((char *)cur_node.nodes[3].test_cases[2].expected_output, "reset ok") == 0);

    assert(cur_node.nodes_count == 4);

    cur_node = t_file[3];
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

    assert(strcmp((char *)cur_node.nodes[0].test_cases[2].test_data, "[in] gg") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[2].expected_output, "[out] gg") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[3].test_data, "Good bye!") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[3].expected_output, "Bye!") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[4].test_data, "ping") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[4].expected_output, "pong") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[5].test_data, "echo test") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[5].expected_output, "echo test") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[6].test_data, "status") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[6].expected_output, "ok") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[7].test_data, "version") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[7].expected_output, "1.0") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[8].test_data, "name") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[8].expected_output, "udp-test-server") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[9].test_data, "help") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[9].expected_output, "commands: ping, status, version, name, help, quit") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[10].test_data, "invalid command") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[10].expected_output, "error: unknown command") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[11].test_data, "") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[11].expected_output, "error: empty message") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[12].test_data, "12345") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[12].expected_output, "54321") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[13].test_data, "UPPERCASE") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[13].expected_output, "uppercase") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[14].test_data, "lowercase") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[14].expected_output, "LOWERCASE") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[15].test_data, "spaces   inside") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[15].expected_output, "spaces   inside") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[16].test_data, "special !@#$%^&*()") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[16].expected_output, "special !@#$%^&*()") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[17].test_data, "json {\"msg\":\"hello\"}") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[17].expected_output, "json ok") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[18].test_data, "long message abcdefghijklmnopqrstuvwxyz") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[18].expected_output, "long message received") == 0);

    assert(strcmp((char *)cur_node.nodes[0].test_cases[19].test_data, "quit") == 0);
    assert(strcmp((char *)cur_node.nodes[0].test_cases[19].expected_output, "closing") == 0);

    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].test_data, "Good bye!") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[0].expected_output, "Bye!") == 0);

    assert(strcmp((char *)cur_node.nodes[1].test_cases[1].test_data, "ping") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[1].expected_output, "pong") == 0);

    assert(strcmp((char *)cur_node.nodes[1].test_cases[2].test_data, "connect") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[2].expected_output, "connected") == 0);

    assert(strcmp((char *)cur_node.nodes[1].test_cases[3].test_data, "disconnect") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[3].expected_output, "disconnected") == 0);

    assert(strcmp((char *)cur_node.nodes[1].test_cases[4].test_data, "reconnect") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[4].expected_output, "reconnected") == 0);

    assert(strcmp((char *)cur_node.nodes[1].test_cases[5].test_data, "data one") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[5].expected_output, "received one") == 0);

    assert(strcmp((char *)cur_node.nodes[1].test_cases[6].test_data, "data two") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[6].expected_output, "received two") == 0);

    assert(strcmp((char *)cur_node.nodes[1].test_cases[7].test_data, "data three") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[7].expected_output, "received three") == 0);

    assert(strcmp((char *)cur_node.nodes[1].test_cases[8].test_data, "reset") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[8].expected_output, "reset ok") == 0);

    assert(strcmp((char *)cur_node.nodes[1].test_cases[9].test_data, "final") == 0);
    assert(strcmp((char *)cur_node.nodes[1].test_cases[9].expected_output, "done") == 0);

    assert(cur_node.nodes_count == 2);
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