#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void free_files(char **files, size_t files_capacity)
{
    for(size_t i = 0; i < files_capacity; i++)
    {
        free(files[i]);
    }    
    free(files);
}


int main(int argc, char *argv[])
{
    // Open every .yaml file inside _net_test
    // Parse .yaml data into structs
    //  - config
    //  - target  
    //  - test
}