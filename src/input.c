#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "input.h"
#include "utils.h"


#define MAX_FILE_PATH 200       // Maximum length of a file path.
#define MAX_CMD 300             // Maximum length of a command.
#define MAX_OPTIONS_LENGTH 20   // Maximum length reserved for options to the user.
#define MAX_NAME 30             // Maximum length of a username/groupname.

int get_yes_no_input(char *prompt) {
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

int get_user_input(char *prompt, char *buffer, size_t bufferSize) {
    int result = 0;
    printf("%s", prompt);
    fflush(stdout); // Ensure the prompt is printed before reading input
    if (fgets(buffer, bufferSize, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        if (strlen(buffer) > 0) {
            result = 1; // Non-empty valid input
        } else {
            result = 0; // Empty valid input
        }
    } else {
        printf("Error reading input.\n");
        result = -1; // Invalid input
    }
    return result;
}

bool get_filepath(char *path) {
    if (get_user_input("Please enter the file path: ", path, MAX_FILE_PATH) == 1) {
        return path_exists(path);
    } else {
        printf("ERR: Invalid/non-existent path.\n");
        return false;
    }
}

bool get_option(char *option) {
    if (get_user_input("MSG: Please insert options prefixed by a SINGLE '-', leave blank for none:", option, MAX_OPTIONS_LENGTH) != -1) {
        return sanitize_options(option);
    } else {
        printf("ERR: Invalid options.\n");
        return false;
    }
}