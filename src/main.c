#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * @brief Structure that contains a list of integers,
 * and the length of that list.
 *
 * This struct is used to collect line numbers where a query matches.
 */
typedef struct RESULT_STRUCT {
    int* line_numbers;
    size_t size;
} result_t;


/** 
 * Converts a char to a string
 *
 * @param char c
 * @return char*
 */
char* char_to_string(char c)
{
    char* str = calloc(2, sizeof(char));
    str[0] = c;
    str[1] = '\0';

    return str;
}

/**
 * Goes through and compares every line in of file with the `content_query`
 * parameter, then returns a boolean (int).
 *
 * @param const char* file_name
 * @param const char* content_query
 *
 * @return int
 */
result_t* file_line_by_line_compare(const char* file_name, const char* content_query)
{
    FILE *fp;

    fp = fopen(file_name, "r");

    result_t* result = calloc(1, sizeof(struct RESULT_STRUCT));
    result->size = 0;
    result->line_numbers = calloc(1, sizeof(int));

    if (fp == NULL)
    {
        // We dont care about the error, just continue...
        return result;
    } 

    int line_counter = 1;  // lines starts at 1 ...
    char line[1024];

    while(fgets(line, sizeof(line), fp))
    {
        if (strstr(line, content_query) != NULL)
        {
            result->size += 1;
            result->line_numbers = realloc(result->line_numbers, result->size * sizeof(int));
            result->line_numbers[result->size - 1] = line_counter;
        }

        line_counter += 1;
    }

    fclose(fp);

    return result;
}

/**
 * Performs a file query based on the name of the file and the contents.
 *
 * This method only prints to stdout.
 *
 * @param const char* name
 * @param const char* name_query
 * @param const char* content_query
 *
 * @return int
 */
int do_query(const char *name, const char* name_query, const char* content_query)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            // current entry is a directory
            
            char path[1024];

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            do_query(path, name_query, content_query);
        }
        else
        {
            // current entry is a file
            
            char* full_path = calloc(strlen(name) + strlen(entry->d_name) + 2, sizeof(char));
            sprintf(full_path, "%s/%s", name, entry->d_name);

            // comparing the name
            if (strstr(full_path, name_query) == NULL)
            {
                free(full_path);
                continue;
            }

            // comparing the content
            result_t* result = file_line_by_line_compare(full_path, content_query);

            // print result
            for (int i = 0; i < result->size; i++)
                printf("%s:%d\n", full_path, result->line_numbers[i]);

            free(result->line_numbers);
            free(result);
            free(full_path);
        }
    }

    closedir(dir);

    return 1;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Usage: qf <name_query> <content_query>\n");
        exit(1);
    }

    if (!do_query(".", argv[1], argv[2]))
        return 1;

    return 0;
}
