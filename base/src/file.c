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

// UNTESTED
bool is_contained(const char* path1, const char* path2) {
    return strncmp(path1, path2, strlen(path1)) == 0;
}

bool find_string_in_file(const char *target, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    char line[1024];
    bool found = false;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, target) != NULL) {
            found = true;
            break;
        }
    }

    fclose(file);
    return found;
}

bool copy_file(const char *source, const char *destination) {
    FILE *source_file = fopen(source, "rb");
    if (source_file == NULL) {
        perror("Error opening source file");
        return false;
    }

    FILE *destination_file = fopen(destination, "wb");
    if (destination_file == NULL) {
        perror("Error opening destination file");
        fclose(source_file);
        return false;
    }

    char buffer[MAX_LINE_LENGTH];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
        fwrite(buffer, 1, bytes_read, destination_file);
    }

    fclose(source_file);
    fclose(destination_file);

    return true;
}

char* find_suffix(const char* filepath, const char* prefix) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* fileContent = malloc(fileSize + 1);
    if (fileContent == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }
    size_t bytesRead = fread(fileContent, 1, fileSize, file);
    if (bytesRead != fileSize) {
        perror("Error reading file");
        free(fileContent);
        fclose(file);
        return NULL;
    }
    fileContent[fileSize] = '\0'; // Null-terminate the string
    fclose(file);
    char* prefixPos = strstr(fileContent, prefix);
    if (prefixPos == NULL) {
        free(fileContent);
        return NULL;
    }
    prefixPos += strlen(prefix);
    if (*prefixPos != ' ') {
        free(fileContent);
        return NULL;
    }
    prefixPos++;
    char* suffixEnd = strchr(prefixPos, '\n');
    if (suffixEnd != NULL) {
        *suffixEnd = '\0'; // Terminate the suffix at the next newline
    }
    char* suffix = strdup(prefixPos);
    if (suffix == NULL) {
        perror("Error allocating memory for suffix");
        free(fileContent);
        return NULL;
    }
    free(fileContent);
    return suffix;
}

bool find_exec_in_file(const char* prefix, const char* filepath){
    char* result = find_suffix(filepath, prefix);
    if(result != NULL){
        return true;
    }else{
        return false;
    }
}