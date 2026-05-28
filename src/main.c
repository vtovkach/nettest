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

int insert_new_file(char ***files, size_t *files_capacity, 
                    const char *str, size_t str_idx )
{
    if(files == NULL || *files == NULL) return -1; 
    if(files_capacity == NULL || *files_capacity == 0) return -1;

    if(str_idx >= *files_capacity)
    {
        size_t new_capacity = (2 * (*files_capacity));

        void **temp = realloc(*files, sizeof(char *) * new_capacity);
        if(temp == NULL)
            return -1;            

        *files = (char **)temp;
        *files_capacity = new_capacity;
    }

    (*files)[str_idx] = strdup(str);
    if((*files)[str_idx] == NULL)
        return -1;

    return 0;
}

static int is_yaml_file(const char *file_name)
{
    const char *ext = strrchr(file_name, '.');
    return ext && (strcmp(ext, ".yaml") == 0 || strcmp(ext, ".yml") == 0);
}

static int build_path(char *buf, size_t buf_size,
                      const char *dir_path, const char *file_name)
{
    int n = snprintf(buf, buf_size, "%s/%s", dir_path, file_name);
    return (n > 0 && (size_t)n < buf_size) ? 0 : -1;
}

int retrieve_test_files(const char *dir_path, char ***out_files)
{
    if(!dir_path || !out_files) return -1;

    DIR *net_test_dir = opendir(dir_path);
    if(!net_test_dir)
    {
        perror("Failed to open tests directory");
        return -1;
    }

    size_t files_capacity = 1; 
    char **files = malloc(sizeof(char *));
    if(!files)
    {
        perror("Failed to allocate memory");
        closedir(net_test_dir);
        return -1;
    }

    int str_idx = 0;
    struct dirent *dir_entry;

    while((dir_entry = readdir(net_test_dir)) != NULL)
    {
        const char *file_name = dir_entry->d_name;

        if(!is_yaml_file(file_name))
            continue;

        size_t path_len = strlen(dir_path) + strlen(file_name) + 2;
        char path[path_len];

        if(build_path(path, path_len, dir_path, file_name) == -1)
        {
            free_files(files, str_idx);
            closedir(net_test_dir);
            return -1;
        }

        if(insert_new_file(&files, &files_capacity, path, str_idx) == -1)
        {
            free_files(files, str_idx);
            closedir(net_test_dir);
            return -1;
        }

        str_idx++;
    }
    closedir(net_test_dir);

    *out_files = files;
    return str_idx;
}

int main(int argc, char *argv[])
{
    // Open every .yaml file inside _net_test
    // Parse .yaml data into structs
    //  - config
    //  - target  
    //  - test

    // find and parse every .yaml file in _net_test/
    // Create all necessary structures 
    // Display status

    // TODO: 
    //  Enforce Max File and Dir Size

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

    // Verify output
    for(int i = 0; i < num_files; i++)
    {
        printf("file: %s\n", files[i]);
    }

    return 0;
}