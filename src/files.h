#ifndef _FILES_
#define _FILES_

#include <stddef.h>

void free_files(char **files, size_t files_capacity);

int insert_new_file(char ***files, size_t *files_capacity, 
                    const char *str, size_t str_idx );

int retrieve_test_files(const char *dir_path, char ***out_files);

#endif