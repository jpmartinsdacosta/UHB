#include <stdbool.h>
#include <stddef.h>
#include "global_var.h"

#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

/**
 * @file input_output.h
 * @brief This file provides headers for functions regarding user input.
 */

/**
 * @brief Function that prompts the user for a yes/no input.
 * @param prompt Custom prompt to show the user.
 * @return 0 if true (yes), 1 if false (no).
 */
int get_yes_no_input(const char *prompt);

/**
 * @brief Function that prompts the user for 3 possible inputs.
 * @param prompt Custom prompt to show the user.
 * @param opt1 Custom valid character option 1;
 * @param opt2 Custom valid character option 2;
 * @param opt3 Custom valid character option 3;
 * @returns -1 on error, or 0, 1 or 2 respectively.
 */
int three_option_input(const char *prompt, const char opt1, const char opt2, const char opt3);

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
 * @brief Get the user's input for a path and checks if it exists.
 * @param path The path to store the user's input.
 * @return true if the user's input is valid, false otherwise.
 */
bool get_filepath(char *path);

/**
 * @brief Parses the user's input and retrieves the token after a given target one.
 * @param input Input to be parsed.
 * @param delimiter The delimiter in string format in which to divide the input in tokens.
 * @param target The target token.
 * @param result The result string.
 * @param resultSize The size of the result string buffer.
 * @returns 0 in failure, 1 in success.
 */
int parse_input_next_token(char *input, const char *delimiter, const char *target, char *result, size_t resultSize);

/**
 * @brief Parses the user's input and retrieves the n-th token.
 * @param input Input to be parsed.
 * @param delimiter The delimiter in string format in which to divide the input in tokens.
 * @param index The index of the token that is to be retrieved.
 * @param result The result string.
 * @param resultSize The size of the result string buffer.
 * @returns 0 in failure, 1 in success.
 */
int parse_input_index_token(char *input, const char *delimiter, int index, char *result, size_t resultSize);

bool is_empty_input(const char *string);

#endif // INPUT_OUTPUT_H