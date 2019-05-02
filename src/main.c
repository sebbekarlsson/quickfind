#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define ERROR -1


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
int file_line_by_line_compare(const char* file_name, const char* content_query)
{
    FILE *fp;

    fp = fopen(file_name, "r");

    if (fp == NULL)
    {
        // We dont care about the error, just continue...
        //
        // perror("Could not open file.\n");
        // printf("Filename: %s\n", file_name);
        // fclose(fp);
        return ERROR;
    }

    int line_counter = 1;  // lines starts at 1 ...
    char line[1024];

    while(fgets(line, sizeof(line), fp))
    {
        if (strstr(line, content_query) != NULL)
        {
            fclose(fp);
            return line_counter;
        }

        line_counter += 1;
    }

    fclose(fp);

    return ERROR;
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
            
            char* full = calloc(strlen(name) + strlen(entry->d_name) + 2, sizeof(char));
            sprintf(full, "%s/%s", name, entry->d_name);

            // comparing the name
            if (strstr(full, name_query) == NULL)
            {
                free(full);
                continue;
            }

            // comparing the content
            int line_number;
            if ((line_number = file_line_by_line_compare(full, content_query)) != ERROR)
            {
                printf("%s:%d\n", full, line_number);
            }

            free(full);
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

    return do_query(".", argv[1], argv[2]);
}
