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

/**
 * ACL Functions to be added:
 */

void parse_acl_entry(const char *entry, char *typeBuf, size_t typeSize,
                     char *nameBuf, size_t nameSize,
                     char *permBuf, size_t permSize) {
    char temp[512];
    strncpy(temp, entry, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char *token = strtok(temp, ":");
    if (token) strncpy(typeBuf, token, typeSize - 1);
    typeBuf[typeSize - 1] = '\0';

    token = strtok(NULL, ":");
    if (token) strncpy(nameBuf, token, nameSize - 1);
    nameBuf[nameSize - 1] = '\0';

    token = strtok(NULL, ":");
    if (token) strncpy(permBuf, token, permSize - 1);
    permBuf[permSize - 1] = '\0';
}

// Extract ACL entries following -m, -x, etc.
int extract_acl_entries(const char *input, const char *flag, char *aclBuf, size_t aclBufSize) {
    char inputCopy[MAX_LINE_LENGTH];
    strncpy(inputCopy, input, sizeof(inputCopy));
    inputCopy[sizeof(inputCopy) - 1] = '\0';

    char *token = strtok(inputCopy, " ");
    while (token) {
        if (strcmp(token, flag) == 0) {
            token = strtok(NULL, " ");
            if (token) {
                strncpy(aclBuf, token, aclBufSize - 1);
                aclBuf[aclBufSize - 1] = '\0';
                return 1;
            }
        }
        token = strtok(NULL, " ");
    }
    aclBuf[0] = '\0';
    return 0;
}

// Find the last token (commonly used for the target path)
int extract_target_path(const char *input, char *targetBuf, size_t targetBufSize) {
    char inputCopy[MAX_LINE_LENGTH];
    strncpy(inputCopy, input, sizeof(inputCopy));
    inputCopy[sizeof(inputCopy) - 1] = '\0';

    char *token = strtok(inputCopy, " ");
    char *lastToken = NULL;
    while (token) {
        lastToken = token;
        token = strtok(NULL, " ");
    }

    if (lastToken) {
        strncpy(targetBuf, lastToken, targetBufSize - 1);
        targetBuf[targetBufSize - 1] = '\0';
        return 1;
    }

    targetBuf[0] = '\0';
    return 0;
}

// Main parser function
bool parse_setfacl_command(const char *input,
                           char *aclFlag, size_t aclFlagSize,
                           char *aclEntryBuf, size_t aclEntryBufSize,
                           char *targetPathBuf, size_t targetPathBufSize) {
    const char *flags[] = { "-m", "-x", "--set", "-d", NULL };
    const char *flagFound = NULL;
    const char *entryStart = NULL;
    const char *lastSpace = NULL;

    // Step 1: Find ACL flag
    for (int i = 0; flags[i] != NULL; ++i) {
        const char *pos = strstr(input, flags[i]);
        if (pos && (pos == input || isspace(*(pos - 1)))) {
            flagFound = flags[i];
            entryStart = pos + strlen(flagFound);
            while (*entryStart && isspace(*entryStart)) entryStart++;  // Skip spaces
            break;
        }
    }

    if (!flagFound || !entryStart) {
        aclFlag[0] = '\0';
        aclEntryBuf[0] = '\0';
        targetPathBuf[0] = '\0';
        return false;
    }

    // Step 2: Extract ACL flag
    strncpy(aclFlag, flagFound, aclFlagSize - 1);
    aclFlag[aclFlagSize - 1] = '\0';

    // Step 3: Extract target path (last token)
    const char *p = input;
    while (*p) {
        if (isspace(*p)) lastSpace = p;
        ++p;
    }

    const char *filePathStart = lastSpace ? lastSpace + 1 : NULL;

    if (filePathStart && *filePathStart != '\0') {
        strncpy(targetPathBuf, filePathStart, targetPathBufSize - 1);
        targetPathBuf[targetPathBufSize - 1] = '\0';
    } else {
        targetPathBuf[0] = '\0';
    }

    // Step 4: Extract ACL entry (everything between flag and filepath)
    size_t entryLen = 0;
    if (filePathStart > entryStart) {
        entryLen = (size_t)(filePathStart - entryStart);
    } else {
        entryLen = strlen(entryStart);  // fallback if no file path
    }

    if (entryLen >= aclEntryBufSize) entryLen = aclEntryBufSize - 1;
    strncpy(aclEntryBuf, entryStart, entryLen);
    aclEntryBuf[entryLen] = '\0';

    return true;
}