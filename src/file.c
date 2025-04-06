#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"
#include "file.h"
#include "acl.h"
#include "config.h"
#include "utils.h"
#include "input.h"

#define LINE_MAX 256
#define MAX_CMD 300

int find_first_in_file(char *target, char *filepath){
    char line[LINE_MAX];
    int position = 0;
    bool found = false;
    if(!path_exists(filepath)){
        printf("ERR: The selected file does not exist.\n");
        return -1;
    }else{
        FILE *file = fopen(filepath, "r");
        if (file) {
            while (fgets(line, LINE_MAX, file) && !found) {
                if (strstr(line, target)) {
                    found = true;
                }else{
                    position++;
                }                    
            }
            fclose(file);
            return found ? position : -1;
        }else{
            printf("ERR: Error reading %s file.\n",filepath);
            return -1;
        }
    }
}

bool find_strings_in_line(char *a, char *b, char *filepath) {
    char line[LINE_MAX];
    int position = 0;

    if (!path_exists(filepath)) {
        printf("ERR: The selected file does not exist.\n");
        return false;
    } else {
        FILE *file = fopen(filepath, "r");
        if (file) {
            while (fgets(line, LINE_MAX, file)) {
                if (strstr(line, a) && strstr(line, b)) {
                    printf("MSG: Both strings found on line %d\n", position);
                    fclose(file);
                    return true;
                }
                position++;
            }
            fclose(file);
            return false;
        } else {
            printf("ERR: Error reading %s file.\n",filepath);
            return false;
        }
    }
}

char* find_fs_from_path(char *filepath){
    char command[MAX_CMD];
    char *result = malloc(MAX_CMD * sizeof(char));
    if (result == NULL) {
        printf("ERR: Memory allocation failed.\n");
        return NULL;
    }
    snprintf(command, sizeof(command), "df %s | tail -n 1 | awk '{print $1}'", filepath);
    FILE *pipe = popen(command, "r");
    if (pipe == NULL) {
        perror("popen");
        free(result); // Free the allocated memory before returning
        return NULL;
    }
    if (fgets(result, MAX_CMD, pipe) != NULL) {
        result[strcspn(result, "\n")] = 0; // Remove newline character

    } else {
        free(result); // Free the allocated memory if no data is read
        result = NULL; // Set to NULL to indicate failure
    }
    pclose(pipe);
    return result;
}

/*
char* find_fs_from_path(char* filepath) {
    char command[MAX_CMD];
    static char output[MAX_CMD]; // Use static to ensure the memory persists after the function returns
    snprintf(command, sizeof(command), "df %s | tail -n 1 | awk '{print $1}'", filepath);
    FILE* pipe = popen(command, "r");
    if (pipe == NULL) {
        perror("popen");
        return NULL;
    }
    if (fgets(output, MAX_CMD, pipe) != NULL) {
        output[strcspn(output, "\n")] = 0; // Remove newline character
    } else {
        output[0] = '\0'; // Ensure output is empty if no data is read
    }
    pclose(pipe);
    return output;
}
*/
