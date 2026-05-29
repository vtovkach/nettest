#include "files.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("main: incorrect args\n");
        return 1;
    }

    char *tests_path = argv[1];
    char **files = NULL; 

    int num_files = retrieve_test_files(tests_path, &files); 
    if(num_files < 1)
    {
        printf("Failed to open \"%s\" directory\n", tests_path);
        return 1; 
    }
    
    return 0;
}