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

bool find_string_in_file(const char *target, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    char line[MAX_LINE_LENGTH];
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

int find_string_in_file_number(const char *filepath, const char *search_string) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    char line[MAX_LINE_LENGTH]; // Buffer to store each line
    int line_number = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        // Check if the search string is present in the current line
        if (strstr(line, search_string) != NULL) {
            fclose(file);
            return line_number;
        }
    }

    fclose(file);
    return -1; // String not found
}

bool replace_line_in_file(const char *filepath, const char *new_line, int line_number) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    // Create a temporary file
    char temp_filepath[256];
    snprintf(temp_filepath, sizeof(temp_filepath), "%s.temp", filepath);
    FILE *temp_file = fopen(temp_filepath, "w");
    if (temp_file == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        return false;
    }

    char line[MAX_LINE_LENGTH]; // Buffer to store each line
    int current_line = 1;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (current_line == line_number) {
            // Replace the line with the new line
            fputs(new_line, temp_file);
            fputs("\n", temp_file); // Ensure the line ends with a newline
        } else {
            // Copy the original line to the temporary file
            fputs(line, temp_file);
        }
        current_line++;
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the temporary file
    if (remove(filepath) != 0) {
        perror("Error deleting original file");
        return false;
    }

    if (rename(temp_filepath, filepath) != 0) {
        perror("Error renaming temporary file");
        return false;
    }

    return true;
}

bool find_and_replace(const char *original, const char *replacement, const char *filepath){
    int pos = find_string_in_file_number(filepath,original);
    if(pos == -1){
        return false;
    }else{
        return replace_line_in_file(filepath,replacement,pos);
    }
}

bool append_to_file(const char *text, const char *filepath){
    FILE *file = fopen(filepath, "a");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }
    fprintf(file, "%s", text);
    fclose(file);
    return true;
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