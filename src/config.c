#include <stdlib.h>
#include <stdio.h>
#include <yaml.h>

#include "config.h"

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