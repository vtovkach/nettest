#include "files.h"
#include "config.h"
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

    struct test_file *iso_tests = NULL;

    int num_parsed_files = parse_yaml_files(files, num_files, &iso_tests);
    // Check return value
    if(num_parsed_files == -1)
    {
        printf("Failed to parse yaml files!\n");
        free_files(files, num_files);
        return 1;
    }

    // Deallocate files strings
    free_files(files, num_files);
    free_parsed_data(iso_tests, num_parsed_files);
    return 0;
}