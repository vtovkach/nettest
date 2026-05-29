#include <stdlib.h>
#include <stdio.h>
#include <yaml.h>

#include "config.h"

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