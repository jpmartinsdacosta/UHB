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
        snprintf(command, sizeof(command), "cat %s | less ; clear", filepath);
        system(command);
        system("clear");
    }
}

bool create_file(const char *filepath){
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        perror("Error creating file");
        return -1;
    }
    fclose(file);
    return 0;
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

int find_first_string_in_file_number(const char *filepath, const char *search_string) {
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

bool find_string_in_file_number(const char *filepath, const char *search_string, int line_number) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    char buffer[MAX_LINE_LENGTH];
    int current_line = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        current_line++;

        if (current_line == line_number) {
            // Check if the search string is present in the current line
            if (strstr(buffer, search_string) != NULL) {
                fclose(file);
                return true;
            } else {
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);
    return false; // Line number not found in the file
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
    int pos = find_first_string_in_file_number(filepath,original);
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
    fprintf(file, "\n%s\n", text);
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

void replace_option_value(const char *option_name, char separator, const char *param, const char *filepath) {
    FILE *file = fopen(filepath, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[MAX_LINE_LENGTH];
    long int pos = 0;
    int option_name_length = strlen(option_name);
    int found = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        pos = ftell(file);

        // Remove the newline character from buffer, if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Check if the line starts with option_name followed by the separator
        if (strncmp(buffer, option_name, option_name_length) == 0 &&
            buffer[option_name_length] == ' ' &&
            buffer[option_name_length + 1] == separator) {

            // Move the file pointer to the beginning of the line
            fseek(file, pos - strlen(buffer) - 1, SEEK_SET);

            // Calculate the length of the new line
            int new_line_length = snprintf(NULL, 0, "%s %c %s", option_name, separator, param);

            // Write the new line with the updated parameter, followed by spaces to overwrite any remaining characters
            fprintf(file, "%s %c %s", option_name, separator, param);
            for (int i = new_line_length; i < strlen(buffer); i++) {
                fputc(' ', file);
            }

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Option '%s' not found in the file.\n", option_name);
    }

    fclose(file);
}

int replace_string_in_line(const char *filepath, int line, const char *target, const char *replacement) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    // Temporary file to write modified content
    char tempFilePath[256];
    snprintf(tempFilePath, sizeof(tempFilePath), "%s.tmp", filepath);
    FILE *tempFile = fopen(tempFilePath, "w");
    if (!tempFile) {
        perror("Error creating temporary file");
        fclose(file);
        return -1;
    }

    char buffer[1024];
    int currentLine = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (currentLine == line) {
            char *pos = strstr(buffer, target);
            if (pos) {
                char *rest = pos + strlen(target);
                *pos = '\0';
                fprintf(tempFile, "%s%s%s", buffer, replacement, rest);
            } else {
                fputs(buffer, tempFile);
            }
        } else {
            fputs(buffer, tempFile);
        }
        currentLine++;
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the temporary file
    if (remove(filepath) != 0) {
        perror("Error deleting original file");
        return -1;
    }

    if (rename(tempFilePath, filepath) != 0) {
        perror("Error renaming temporary file");
        return -1;
    }

    return 0;
}

int smart_replacement(const char *prompt, const char *filepath, int line, const char *target){
    char replacement[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input(prompt,replacement,sizeof(replacement));
        opt = three_option_input("MSG: Is this information correct? (Y)es/(N)o/E(x)it:",'Y','N','X');
    }
    if(opt == 0){
        if(find_string_in_file_number(filepath,target,line)){
            replace_string_in_line(filepath,line,target,replacement);
            return 0;
        }else{
            return -1;
        }
    }else if(opt == 2){
        return 1;
    }
}
