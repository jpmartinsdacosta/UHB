#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "menu.h"
#include "file.h"
#include "config.h"
#include "utils.h"
#include "os_interface.h"
#include "global_var.h"

int get_yes_no_input(const char *prompt) {
    char input[3];
    int answer = -1;
    printf("%s", prompt);
    while (answer == -1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = 0; // Remove newline character
            for (int i = 0; input[i]; i++) {
                input[i] = tolower(input[i]);
            }
            if (strcmp(input, "y") == 0) {
                answer = 0;
            } else if (strcmp(input, "n") == 0) {
                answer = 1;
            } else {
                printf("Invalid input. Please enter 'y' or 'n'.\n");
            }
        }
    }
    return answer;
}

int get_user_input(const char *prompt, char *buffer, size_t bufferSize) {
    int result = 0;
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buffer, bufferSize, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0) {
            result = 1;
        } else {
            result = 0;
        }
    } else {
        fprintf(stderr, "ERR: get_user_input(): Error reading input.\n");
        result = -1;
    }
    return result;
}

bool get_filepath(char *path) {
    if (get_user_input("Please enter the file path: ", path, MAX_FILEPATH_SIZE) == 1) {
        return path_exists(path);
    } else {
        fprintf(stderr, "ERR: get_filepath(): Invalid/non-existent path.\n");
        return false;
    }
}

