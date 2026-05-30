#include <stdlib.h>
#include <stdio.h>
#include <yaml.h>
#include <stdbool.h>
#include <errno.h>

#include "config.h"

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
                        bool *expect_key)
{
    if(*expect_key == true)
    {
        strcpy(cur_key, cur_scalar); // cur_scalar is guaranteed to be <KEY_MAX_SIZE 
        *expect_key = false;
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
                printf("[parse_config] Invalid family. (ipv4 or ipv6). Cur Scalar: %s\n", cur_scalar);
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
        *expect_key = true;
    }
    return 0;
}

static int parse_file(const char *file, struct iso_test_node *arr_dest, size_t dest_idx)
{
    if(!file)
    {
        printf("[parse_file] file is NULL\n");
        return -1; 
    }

    FILE *yaml_file = fopen(file, "r");
    if(!yaml_file)
    {
        perror("[parse_file] failed to open \"%s\" file:");
        return -1;
    }

    yaml_parser_t parser; 
    yaml_parser_set_input_file(&parser, yaml_file);

    int stop = 0;
    yaml_event_t event; 
    while(!stop)
    {
        if(!yaml_parser_parse(&parser, &event))
            goto error;

        /* Process events here */
        // ... 

        /* Is everything parsed? */
        stop = (event.type == YAML_STREAM_END_EVENT);
        yaml_event_delete(&event);
    }

    yaml_parser_delete(&parser);
    return 0; 

    error:
    yaml_parser_delete(&parser);
    return -1;
}

int parse_yaml_files(char **files, size_t files_num, struct iso_test_node **dest)
{
    if(!files) 
    {
        printf("[parse_yaml_file] files is null\n");
        return -1;
    }

    if(dest)
    {
        printf("[parse_yaml_file] dest != NULL; must be NULL\n");
        return -1;
    }

    struct iso_test_node *nodes = malloc(sizeof(struct iso_test_node) * files_num);
    if(!dest)
    {
        perror("[parse_yaml_file] malloc failure:");
        return -1; 
    }

    size_t nodes_count = 0;
    for(size_t i = 0; i < files_num; i++)
    {
        if(parse_file(files[i], nodes, nodes_count) < 0) 
            continue;
        nodes_count++;
    }

    *dest = nodes;
    return nodes_count;
}