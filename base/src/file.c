#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"
#include "file.h"
#include "config.h"
#include "utils.h"
#include "input_output.h"
#include "os_interface.h"
#include "global_var.h"

bool path_exists(const char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return true;
    } else {
        fprintf(stderr, "ERR: path_exists(): File path %s does not exist.\n", path);
        return false;
    }
}

void view_file(const char *filepath) {
    char command[MAX_LINE_LENGTH];
    if (!path_exists(filepath)) {
        fprintf(stderr, "ERR: view_file(): Configuration file does not exist.\n");
    } else {
        printf("MSG: Press q to exit current view.\n");
        snprintf(command, sizeof(command), "cat %s | less", filepath);
        system(command);
    }
}

bool is_contained(const char* path1, const char* path2) {
    return strncmp(path1, path2, strlen(path1)) == 0;
}

int find_first_in_file(const char *target, const char *fp){
    char line[MAX_LINE_LENGTH];
    int position = 0;
    bool found = false;
    if(!path_exists(fp)){
        fprintf(stderr, "ERR: find_first_in_file(): The selected file does not exist.\n");
        return -1;
    }else{
        FILE *file = fopen(fp, "r");
        if (file) {
            while (fgets(line, MAX_LINE_LENGTH, file) && !found) {
                if (strstr(line, target)) {
                    found = true;
                }else{
                    position++;
                }                    
            }
            fclose(file);
            return found ? position : -1;
        }else{
            fprintf(stderr, "ERR: find_first_in_file(): Error reading %s file.\n",fp);
            return -1;
        }
    }
}

bool find_strings_in_line(const char *a, const char *b, const char *fp) {
    char line[MAX_LINE_LENGTH];
    int position = 0;

    if (!path_exists(fp)) {
        return false;
    } else {
        FILE *file = fopen(fp, "r");
        if (file) {
            while (fgets(line, MAX_LINE_LENGTH, file)) {
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
            fprintf(stderr, "ERR: find_strings_in_line(): Error reading %s file.\n",fp);
            return false;
        }
    }
}

char* find_fs_from_path(const char *fp){
    char command[MAX_LINE_LENGTH];
    char *result = malloc(MAX_LINE_LENGTH * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "ERR: find_fs_from_path(): Memory allocation failed.\n");
        return NULL;
    }
    snprintf(command, sizeof(command), "df %s | tail -n 1 | awk '{print $1}'", fp);
    FILE *pipe = popen(command, "r");
    if (pipe == NULL) {
        perror("popen");
        free(result); // Free the allocated memory before returning
        return NULL;
    }
    if (fgets(result, MAX_LINE_LENGTH, pipe) != NULL) {
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

bool find_first_and_replace(const char *fp, const char *target, const char *replacement) {
    FILE *file = fopen(fp, "r");
    FILE *temp = tmpfile();
    char buffer[MAX_LINE_LENGTH];
    bool found = false;

    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    if (temp == NULL) {
        perror("Error creating temp file");
        fclose(file);
        return false;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        if (!found) {
            char *pos = strstr(buffer, target);
            if (pos != NULL) {
                size_t target_len = strlen(target);
                size_t replacement_len = strlen(replacement);
                size_t remaining_len = strlen(pos + target_len);

                // Ensure buffer has enough space for replacement
                if (replacement_len + remaining_len < MAX_LINE_LENGTH - (pos - buffer)) {
                    memmove(pos + replacement_len, pos + target_len, remaining_len + 1);
                    memcpy(pos, replacement, replacement_len);
                    found = true;
                } else {
                    fprintf(stderr, "ERR: find_first_and_replace(): Buffer too small for replacement.\n");
                    fclose(file);
                    fclose(temp);
                    return false;
                }
            }
        }
        fputs(buffer, temp);
    }

    fclose(file);
    file = fopen(fp, "w");
    if (file == NULL) {
        perror("Error reopening file");
        fclose(temp);
        return false;
    }

    rewind(temp);
    while (fgets(buffer, sizeof(buffer), temp)) {
        fputs(buffer, file);
    }

    fclose(temp);
    fclose(file);

    if (!found) {
        fprintf(stderr, "ERR: find_first_and_replace(): String not found.\n");
        return false;
    } else {
        return true;
    }
}

bool find_n_and_replace(const char *fp, const char *target, const char *replacement, int num){
    if(num <= 0){
        fprintf(stderr, "ERR: find_n_and_replace(): Cannot find 0 or less instances of \"%s\" in a file.\n",target);
        return false;
    }else{
        int aux = 0;
        bool possible = true;
        while(aux != num && possible){
            possible = find_first_and_replace(fp,target,replacement);
            if(possible){
                aux++;
            }       
        }
        printf("MSG: Searched: %d, replaced: %d\n",num,aux);
        return true;
    }
}