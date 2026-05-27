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
int main(int argc, char *argv[])
{
    // Open every .yaml file inside _net_test
    // Parse .yaml data into structs
    //  - config
    //  - target  
    //  - test
}