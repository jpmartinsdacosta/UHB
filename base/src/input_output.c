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

int three_option_input(const char *prompt, const char opt1, const char opt2, const char opt3) {
    char input[3];
    int answer = -1;
    printf("%s", prompt);
    while (answer == -1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = 0; // Remove newline character
            for (int i = 0; input[i]; i++) {
                input[i] = tolower(input[i]);
            }
            if (input[0] == tolower(opt1) && input[1] == '\0') {
                answer = 0;
            } else if (input[0] == tolower(opt2) && input[1] == '\0') {
                answer = 1;
            } else if (input[0] == tolower(opt3) && input[1] == '\0') {
                answer = 2;
            } else {
                printf("Invalid input. Please try again.\n");
            }
        } else {
            fprintf(stderr, "Error reading input. Please try again.\n");
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

int parse_input_next_token(char *input, const char *delimiter, const char *target, char *resultBuffer, size_t bufferSize) {
    char inputCopy[MAX_LINE_LENGTH]; // Buffer to store a copy of the input
    strncpy(inputCopy, input, sizeof(inputCopy) - 1);
    inputCopy[sizeof(inputCopy) - 1] = '\0'; // Ensure null-termination

    char *token;
    char *nextToken = NULL;

    // Use strtok to split the input string copy
    token = strtok(inputCopy, delimiter);

    // Loop through the tokens
    while (token != NULL) {
        if (strcmp(token, target) == 0) {
            // If the current token matches the target, get the next token
            nextToken = strtok(NULL, delimiter);
            if (nextToken != NULL) {
                // Copy the next token to the result buffer
                printf("%s: %s\n",target, nextToken);
                strncpy(resultBuffer, nextToken, bufferSize - 1);
                resultBuffer[bufferSize - 1] = '\0'; // Ensure null-termination
                return 1; // Success
            } else {
                // No token after the target
                resultBuffer[0] = '\0'; // Empty string
                return 0; // Failure
            }
        }
        token = strtok(NULL, delimiter);
    }

    // Target not found
    resultBuffer[0] = '\0'; // Empty string
    return 0; // Failure
}

int parse_input_index_token(const char *input, const char *delimiter, int index, char *result, size_t resultSize) {
    char inputCopy[MAX_LINE_LENGTH]; // Buffer to store a copy of the input

    // Create a copy of the input string
    strncpy(inputCopy, input, sizeof(inputCopy) - 1);
    inputCopy[sizeof(inputCopy) - 1] = '\0'; // Ensure null-termination

    char *token;
    int count = 0;

    // Use strtok to split the copied input string
    token = strtok(inputCopy, delimiter);

    while (token != NULL) {
        if (count == index) {
            strncpy(result, token, resultSize - 1);
            result[resultSize - 1] = '\0'; // Ensure null-termination
            return 1; // Success
        }
        count++;
        token = strtok(NULL, delimiter);
    }

    result[0] = '\0'; // Empty string if nth token not found
    return 0; // nth token not found
}

bool is_empty_input(const char *string) {
    return string[0] == '\n' || string[0] == '\0';
}

