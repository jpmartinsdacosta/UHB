#include <stdbool.h>
#include <stddef.h>
#include "global_var.h"

#ifndef INPUT_H
#define INPUT_H

/**
 * @file input.h
 * @brief This file provides headers for functions regarding user input.
 */

/**
 * @brief Function that prompts the user for a yes/no input.
 * @param prompt Custom prompt to show the user.
 * @return 0 if true (yes), 1 if false (no).
 */
int get_yes_no_input(const char *prompt);

/**
 * @brief Get the user's input.
 * @param prompt The prompt to show to the user.
 * @param buffer The buffer to store the user's input.
 * @param bufferSize The size of the buffer.
 * @return 1 if the user's input is valid, 0 if the user's input is empty, -1 if there was an error reading the input.
 * Used for any and all user inputs and prompts throughout the program.
 */
int get_user_input(const char *prompt, char *buffer, size_t bufferSize);

/**
 * @brief Get the user's input for a path.
 * @param path The path to store the user's input.
 * @return true if the user's input is valid, false otherwise.
 */
bool get_filepath(char *path);

/**
 * @brief Get the user's input for DAC options.
 * @param exec The options to store the user's input.
 * @param list List of valid flags to check when executing the command.
 * @return true if the user's input is valid, false otherwise.
 */
bool get_option(char *exec, FlagList *list);

#endif // INPUT_H