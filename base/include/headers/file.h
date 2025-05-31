#include <stdbool.h>

#ifndef FILE_H
#define FILE_H

/**
 * @file file.h
 * @brief This file provides headers for functions regarding general file managment throughout the program.
 */

/**
 * @brief Check if a file exists.
 * @param path The path to the file.
 * @return true if the file exists, false otherwise.
 */
bool path_exists(const char *path);

/**
 * @brief View the contents of a file.
 * @param filepath The filepath of the given file.
 * This function will print the contents of the given file.
 * If the file does not exist, it will print an error message.
 */
void view_file(const char *filepath);

/**
 * @brief Creates an empty file given a filepath.
 * @param filepath The filepath of the file to create.
 * @returns True if successful, false otherwise.
 */
bool create_file(const char *filepath);

/**
 * @brief Finds a given string in a file.
 * @param target String to be found.
 * @param filepath Given file.
 * @returns True if found, false otherwise.
 */
bool find_string_in_file(const char *target, const char *filepath);

/**
 * @brief Finds the line number in which the FIRST instance of a given string is located.
 * @param target Text to be searched.
 * @param filepath The filepath of the given file.
 * @returns -1 if not found or if an error has occured, otherwise returns the line number.
 */
int find_first_string_in_file_number(const char *filepath, const char *search_string);

/**
 * @brief Replaces a line of a file with the given string.
 * @param filepath The filepath of the given file.
 * @param replacement Replacement string.
 * @param line The line number to be replaced.
 * @returns True if successful, false otherwise.
 */
bool replace_line_in_file(const char *filepath, const char *replacement, int line);

/**
 * @brief Finds a string in a given text file and replaces it.
 * @param original Original text to be found.
 * @param replacement Replacement text to be used.
 * @param filepath The filepath of the given file.
 * @returns True if successful, false otherwise.
 */
bool find_and_replace(const char *original, const char *replacement, const char *filepath);

/**
 * @brief Appends text to a given file.
 * @param text Text to be appended.
 * @param filepath Target file.
 * @returns True if successful, false otherwise.
 */
bool append_to_file(const char *text, const char *filepath);

/**
 * @brief Copies a file, character by character to another.
 * @param source        Source fp to be copied.
 * @param destination   Destination fp to be copied to.
 * @returns True if successful, false otherwise.
 */
bool copy_file(const char *source, const char *destination);

/**
 * @brief Finds the suffix of a given prefix in a file.
 * @param filepath Path to the given file.
 * @param prefix Prefix to search.
 * @returns A pointer to the array of characters of the suffix.
 * @note Used to retrieve the executable that is being searched for a service in a UHB Additions Module.
 */
char* find_suffix(const char* filepath, const char* prefix);

/**
 * @brief Checks if an executable exists in a given file.
 * @param prefix The uhb_xxx prefix located in the UHB Additions Module configuration file.
 * @param filepath The path to the UHB Additions Module configuration file.
 * @return True if found, false otherwise.
 */
bool find_exec_in_file(const char* prefix, const char* filepath);

/**
 * @brief This function is used to replace the value of an option in a generic configuration file.
 * @param option_name The name of the option that needs to be modified.
 * @param separator The character used as a separator, generally '=' or ':'
 * @param param The parameter to be used for the option.
 * @param filepath Filepath to the configuration file.
 */
void replace_option_value(const char *option_name, char separator, const char *param, const char *filepath);

int replace_string_in_line(const char *filepath, int line, const char *target, const char *replacement);

/**
 * @brief Auxiliary function that contains most parameters needed to replace a value in a configuration file.
 * It will also ask the user if the information is correct or not, and if it wishes to leave without replacing.
 * @param prompt        The prompt to show the user.
 * @param filepath      The path to the file that needs to be modified.
 * @param int           The line number where the value to be modified is located.
 * @param target        The original string that contains the value that needs to be replaced.
 * @returns 0 if the user has exited the program, 1 if successful, -1 in error.
 */
int smart_replacement(const char *prompt, const char *filepath, int line, const char *target);

#endif // FILE_H