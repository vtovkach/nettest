#include <stdlib.h>
#include <stdio.h>
#include <yaml.h>
#include <stdbool.h>
#include <errno.h>

#include "config.h"

#define KEY_MAX_SIZE 128
#define INIT_NODES_NUM 1
#define INIT_TEST_CASES_NUM 1

typedef enum 
{
    STATE_ROOT,
    STATE_CONFIG,
    STATE_TESTS,
    STATE_TEST,
    STATE_STEPS,
    STATE_TEST_CASE
} parse_state_t;

static const char *yaml_event_type_str(yaml_event_type_t type)
{
    switch (type)
    {
        case YAML_NO_EVENT: return "YAML_NO_EVENT";
        case YAML_STREAM_START_EVENT: return "YAML_STREAM_START_EVENT";
        case YAML_STREAM_END_EVENT: return "YAML_STREAM_END_EVENT";
        case YAML_DOCUMENT_START_EVENT: return "YAML_DOCUMENT_START_EVENT";
        case YAML_DOCUMENT_END_EVENT: return "YAML_DOCUMENT_END_EVENT";
        case YAML_ALIAS_EVENT: return "YAML_ALIAS_EVENT";
        case YAML_SCALAR_EVENT: return "YAML_SCALAR_EVENT";
        case YAML_SEQUENCE_START_EVENT: return "YAML_SEQUENCE_START_EVENT";
        case YAML_SEQUENCE_END_EVENT: return "YAML_SEQUENCE_END_EVENT";
        case YAML_MAPPING_START_EVENT: return "YAML_MAPPING_START_EVENT";
        case YAML_MAPPING_END_EVENT: return "YAML_MAPPING_END_EVENT";
        default: return "UNKNOWN_EVENT";
    }
}

static int parse_port(const char *str, uint16_t *dest_port)
{
    if(!str || !dest_port) return -1;
    if(str[0] == '-') return -1;
    
    errno = 0;
    char *end = NULL;
    unsigned long value = strtoul(str, &end, 10);

    if(errno != 0 || end == str || *end != '\0' || value > UINT16_MAX) 
        return -1;
    
    *dest_port = value;
    return 0;
}

static int parse_config(const char *cur_scalar, 
                        char *cur_key, 
                        struct target *target, 
                        bool *isKey)
{
    if(*isKey == true)
    {
        // cur_scalar is guaranteed to be <KEY_MAX_SIZE 
        strcpy(cur_key, cur_scalar);
        *isKey = false;
    }
    else
    {
        if(strcasecmp(cur_key, "protocol") == 0)
        {
            if(strcasecmp(cur_scalar, "tcp") == 0) 
                target->protocol = PROTOCOL_TCP;
            else if(strcasecmp(cur_scalar, "udp") == 0) 
                target->protocol = PROTOCOL_UDP;
            else 
            {
                printf("[parse_config] Invalid protocol. (TCP or UDP)\n");
                return -1;
            }
        }
        else if(strcasecmp(cur_key, "family") == 0)
        {
            if(strcasecmp(cur_scalar, "ipv4") == 0) 
                target->family = IPV4;
            else if(strcasecmp(cur_scalar, "ipv6") == 0) 
                target->family = IPV6;
            else 
            {
                printf(
                    "[parse_config] Invalid family.\
                     (ipv4 or ipv6). Cur Scalar: %s\n", 
                    cur_scalar);
                return -1;
            }
        }
        else if(strcasecmp(cur_key, "ip_address") == 0)
        {
            if(strlen(cur_scalar) < INET6_ADDRSTRLEN)
                strcpy(target->ip_address, cur_scalar);
            else
            {
                printf("[parse_config] \"ip_address\" is too long.\n");
                return -1;
            }
        }
        else if(strcasecmp(cur_key, "port") == 0)
        {
            if(parse_port(cur_scalar, &target->port) != 0)   
            {
                printf("[parse_config] invalid port: %s\n", cur_scalar);
                return -1;
            }
        }
        else
        {
            printf("Invalid scalar encountered: %s\n", cur_scalar);
            return -1;
        }
        *isKey = true;
    }
    return 0;
}

static int parse_test_case(char *cur_key, const char *cur_scalar, 
                            bool *isKey, struct test_case *dest)
{
    if(*isKey)
    {
        strcpy(cur_key, cur_scalar);
        *isKey = false;
    }
    else
    {
        size_t data_size = strlen(cur_scalar);
        void *generic_data = malloc(data_size + 1);
        if(!generic_data)
        {
            perror("[parse_test_case] malloc failure:");
            return -1;
        } 

        memcpy(generic_data, cur_scalar, data_size);
        ((char *)generic_data)[data_size] = '\0';

        if(strcmp(cur_key, "send") == 0)
        {
            dest->test_data = generic_data;
            dest->test_data_size = data_size; 
        }
        else if(strcmp(cur_key, "expect") == 0)
        {
            dest->expected_output = generic_data;
            dest->expected_size = data_size;
        }
        *isKey = true;
    }

    return 0;
}

static int mapping_start(parse_state_t *parse_state)
{
    DBG_PRINT("YAML_MAPPING_START_EVENT\n");

    if(*parse_state == STATE_STEPS)
    {
        *parse_state = STATE_TEST_CASE;
    }

    return 0;
}

static int mapping_end(parse_state_t *parse_state, struct test_file *t_file, 
                       size_t *cur_test_idx, size_t *cur_node_idx)
{
    DBG_PRINT("YAML_MAPPING_END_EVENT\n");

    if(*parse_state == STATE_CONFIG)
    {
        *parse_state = STATE_ROOT;
    }
    else if(*parse_state == STATE_TEST_CASE)
    {
        *parse_state = STATE_STEPS;
        (*cur_test_idx)++;   
        t_file->nodes[*cur_node_idx].tests_count++;
    }
    else if(*parse_state == STATE_TEST)
    {
        *parse_state = STATE_TESTS;
    }
    
    return 0;
}

static int sequence_start(void)
{
    DBG_PRINT("YAML_SEQUENCE_START_EVENT\n");
    return 0;
}
static int sequence_end(parse_state_t *parse_state, 
                        struct test_file *t_file, size_t *cur_node_idx)
{
    DBG_PRINT("YAML_SEQUENCE_END_EVENT\n");

    if(*parse_state == STATE_STEPS)
    {
        *parse_state = STATE_TEST;
        (*cur_node_idx)++; 
        t_file->nodes_count++;
    }
    else if(*parse_state == STATE_TESTS)
    {
        *parse_state = STATE_ROOT;
    }
    else 
    {
        printf("[sequence_event] unsupported event case");
        return -1;
    }

    return 0;
}

static int scalar_event(const yaml_event_t *event, parse_state_t *parse_state, 
                        char *current_key, struct test_file *test_file, 
                        bool *isKey, size_t *cur_node_idx, 
                        size_t *cur_test_idx)
{
    char *value = (char *)event->data.scalar.value;
    DBG_PRINT("YAML_SCALAR_EVENT: %s\n", value);

    if(*parse_state == STATE_ROOT && (strcmp(value, "config") == 0))
    {
        *parse_state = STATE_CONFIG;
    }
    else if(*parse_state == STATE_CONFIG)
    {
        parse_config(value, current_key, &test_file->target, isKey);
    }
    else if(*parse_state == STATE_ROOT && (strcmp(value, "tests") == 0))
    {
        *parse_state = STATE_TESTS;
    }
    else if(*parse_state == STATE_TESTS && (strcmp(value, "test") == 0))
    {
        *parse_state = STATE_TEST;

        if(test_file->nodes == NULL)
        {
            test_file->nodes = calloc(INIT_NODES_NUM, sizeof(struct test_node));
            test_file->nodes_capacity = INIT_NODES_NUM;
            if(!test_file->nodes) return -1;
            *cur_node_idx = 0;
        }
        else if(*cur_node_idx >= test_file->nodes_capacity)
        {
            size_t new_capacity = test_file->nodes_capacity * 2; 
            struct test_node *temp = realloc(
                test_file->nodes, 
                new_capacity * sizeof(struct test_node)
            );
            if(!temp)
            {
                perror("[scalar_event] realloc failure:");
                return -1;
            }
            test_file->nodes = temp;
            test_file->nodes_capacity = new_capacity;
        }
    }
    else if(*parse_state == STATE_TEST && (strcmp(value, "steps") == 0))
    {
        *parse_state = STATE_STEPS;

        *cur_test_idx = 0;
        if(test_file->nodes[*cur_node_idx].test_cases == NULL)
        {
            test_file->nodes[*cur_node_idx].test_cases = calloc(
                INIT_TEST_CASES_NUM, 
                sizeof(struct test_case)
            );
            test_file->nodes[*cur_node_idx].test_cases_capacity = INIT_TEST_CASES_NUM;
        }
    }
    else if(*parse_state == STATE_TEST_CASE)
    {
        if(*cur_test_idx >= test_file->nodes[*cur_node_idx].test_cases_capacity)
        {
            size_t new_capacity = test_file->nodes[*cur_node_idx].test_cases_capacity * 2;
            struct test_case *temp = realloc(
                test_file->nodes[*cur_node_idx].test_cases, 
                sizeof(struct test_case) * new_capacity
            );
            if(!temp)
            {
                perror("[scalar_event] realloc failure:");
                return -1;
            }
            test_file->nodes[*cur_node_idx].test_cases = temp;
            test_file->nodes[*cur_node_idx].test_cases_capacity = new_capacity;
        }

        parse_test_case(
            current_key, 
            value, 
            isKey, 
            &test_file->nodes[*cur_node_idx].test_cases[*cur_test_idx]
        );
    }
    else
    {
        printf("[scalar_event] unsupported event");
        return -1;
    }
    
    return 0;
}

static int handle_yaml_event(yaml_event_t *event, parse_state_t *parse_state,
                            struct test_file *test_file, char *current_key,
                            bool *isKey, size_t *cur_node_idx,
                            size_t *cur_test_idx, int *stop)
{
    switch(event->type)
    {
        case YAML_STREAM_START_EVENT:
            DBG_PRINT("YAML_STREAM_START_EVENT\n");
            return 0;
        
        case YAML_STREAM_END_EVENT:
            DBG_PRINT("YAML_STREAM_END_EVENT\n");
            *stop = 1;
            return 0;
        
        case YAML_DOCUMENT_START_EVENT:
            DBG_PRINT("YAML_DOCUMENT_START_EVENT\n");
            return 0;

        case YAML_DOCUMENT_END_EVENT:
            DBG_PRINT("YAML_DOCUMENT_END_EVENT\n");
            return 0;

        case YAML_MAPPING_START_EVENT:
            return mapping_start(parse_state);
    
        case YAML_MAPPING_END_EVENT:
            return mapping_end(
                parse_state, 
                test_file, 
                cur_test_idx, 
                cur_node_idx
            );
            
        case YAML_SEQUENCE_START_EVENT:
            return sequence_start();

        case YAML_SEQUENCE_END_EVENT:
            return sequence_end(parse_state, test_file, cur_node_idx);

        case YAML_SCALAR_EVENT:
            return scalar_event(
                event, 
                parse_state, 
                current_key, 
                test_file, 
                isKey, 
                cur_node_idx, 
                cur_test_idx
            );

        default:
            fprintf(
                stderr, "Unsupported yaml event: %s\n", 
                yaml_event_type_str(event->type)
            );
            return -1;
    }
}

static int parse_file(const char *file, struct test_file *arr_dest, 
                        size_t dest_idx)
{
    FILE *yaml_file = NULL;
    yaml_parser_t parser;
    yaml_event_t event;
    bool parser_initialized = false;
    bool event_initialized = false;
    int status = -1;

    struct test_file test_file = {0};
    char current_key[KEY_MAX_SIZE] = {0};
    parse_state_t parse_state = STATE_ROOT;
    bool is_key = true;
    size_t cur_node_idx = 0;
    size_t cur_test_idx = 0;
    int stop = 0;

    if (!file || !arr_dest)
    {
        fprintf(stderr, "[parse_file] invalid argument\n");
        goto error;
    }

    yaml_file = fopen(file, "r");
    if (!yaml_file)
    {
        perror("[parse_file] fopen failed");
        goto error;
    }

    if (!yaml_parser_initialize(&parser))
    {
        fprintf(stderr, "[parse_file] yaml_parser_initialize failed\n");
        goto error;
    }

    parser_initialized = true;
    yaml_parser_set_input_file(&parser, yaml_file);

    while (!stop)
    {
        if (!yaml_parser_parse(&parser, &event))
            goto error;

        event_initialized = true;

        status = handle_yaml_event(
            &event,
            &parse_state,
            &test_file,
            current_key,
            &is_key,
            &cur_node_idx,
            &cur_test_idx,
            &stop
        );

        yaml_event_delete(&event);
        event_initialized = false;

        if (status != 0)
            goto error;
    }

    arr_dest[dest_idx] = test_file;

    yaml_parser_delete(&parser);
    fclose(yaml_file);
    return 0;

error:
    if (event_initialized)
        yaml_event_delete(&event);

    if (parser_initialized)
        yaml_parser_delete(&parser);

    if (yaml_file)
        fclose(yaml_file);

    fprintf(stderr, "[parse_file] failure\n");
    return -1;
}

int parse_yaml_files(char **files, size_t files_num, struct test_file **dest)
{
    if(!files) 
    {
        printf("[parse_yaml_file] files is null\n");
        return -1;
    }

    struct test_file *t_file = malloc(sizeof(struct test_file) * files_num);
    if(!dest)
    {
        perror("[parse_yaml_file] malloc failure:");
        return -1; 
    }

    size_t nodes_count = 0;
    for(size_t i = 0; i < files_num; i++)
    {
        if(parse_file(files[i], t_file, nodes_count) < 0) 
            return -1;
        nodes_count++;
    }

    *dest = t_file;
    return nodes_count;
}