#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Function that receives a yes/no user input.
 * @param prompt Custom prompt to show the user.
 * @return 0 if true (yes), 1 if false (no).
 */

int get_yes_no_input(char *prompt);

/**
 * @brief Get the user's input.
 * @param prompt The prompt to show to the user.
 * @param buffer The buffer to store the user's input.
 * @param bufferSize The size of the buffer.
 * @return 1 if the user's input is valid, 0 if the user's input is empty, -1 if there was an error reading the input.
 * Used for any and all user inputs throughout the program.
 */

int get_user_input(char *prompt, char *buffer, size_t bufferSize);

/**
 * @brief Get the user's input for a path.
 * @param path The path to store the user's input.
 * @return true if the user's input is valid, false otherwise.
 */

bool get_filepath(char *path);

/**
 * @brief Get the user's input for DAC options.
 * @param option The options to store the user's input.
 * @return true if the user's input is valid, false otherwise.
 */

bool get_option(char *option);

#endif // INPUT_H