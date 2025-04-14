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
#include "os_interface.h"

#define LINE_MAX 256
#define MAX_CMD 300

bool path_exists(const char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return true;
    } else {
        printf("MSG: File path %s does not exist.\n", path);
        return false;
    }
}

int find_first_in_file(const char *target, const char *filepath){
    char line[LINE_MAX];
    int position = 0;
    bool found = false;
    if(!path_exists(filepath)){
        printf("ERR: find_first_in_file(): The selected file does not exist.\n");
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
            printf("ERR: find_first_in_file(): Error reading %s file.\n",filepath);
            return -1;
        }
    }
}

bool find_strings_in_line(const char *a, const char *b, const char *filepath) {
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

char* find_fs_from_path(const char *filepath){
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

bool copy_file(const char *source, const char *destination) {
    FILE *src = fopen(source, "r");
    if (src == NULL) {
        return false;
    }

    FILE *dst = fopen(destination, "w");
    if (dst == NULL) {
        fclose(src);
        return false;
    }

    int ch;
    while ((ch = fgetc(src)) != EOF) {
        if (fputc(ch, dst) == EOF) {
            fclose(src);
            fclose(dst);
            return false;
        }
    }

    fclose(src);
    if (fclose(dst) == EOF) {
        return false;
    }

    return true;
}

bool find_first_and_replace(const char *filepath, const char *target, const char *replacement) {
    FILE *file = fopen(filepath,"r");
    FILE *temp = tmpfile();
    char buffer[LINE_MAX];
    bool found = false;
    if(file == NULL){
        perror("perror");
        return false;
    }
    if(temp == NULL){
        perror("perror");
        fclose(file);
        return false;
    }
    while(fgets(buffer,sizeof(buffer),file)){
        if(!found){
            char *pos = strstr(buffer, target);
            if(pos != NULL){
                memmove(pos + strlen(replacement), pos + strlen(target), strlen(pos + strlen(target)) + 1);
                memcpy(pos, replacement, strlen(replacement));
                found = true;
            }
        }
        fputs(buffer, temp);
    }
    fclose(file);
    file = fopen(filepath,"w");
    if(file == NULL){
        perror("perror");
        fclose(temp);
        return false;
    }
    rewind(temp);
    while(fgets(buffer,sizeof(buffer),temp)){
        fputs(buffer,file);
    }
    fclose(temp);
    fclose(file);
    if(!found){
        printf("String not found\n");
        return false;
    }else{
        return true;
    }
}

bool find_n_and_replace(const char *filepath, const char *target, const char *replacement, int num){
    if(num <= 0){
        printf("ERR: Cannot find 0 or less instances of \"%s\" in a file.\n",target);
        return false;
    }else{
        int aux = 0;
        bool possible = true;
        while(aux != num && possible){
            possible = find_first_and_replace(filepath,target,replacement);
            if(possible){
                aux++;
            }       
        }
        printf("MSG: Searched: %d, replaced: %d\n",num,aux);
        return true;
    }
}