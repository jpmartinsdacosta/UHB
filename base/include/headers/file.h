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
 * @brief Checks if path1 is a prefix of path2.
 * @param path1 Path that may or may not contain path2.
 * @param path2 Path that is to be checked if is inside path1.
 * @note 1. This function needs both paths to be absolute, as it compares one path with another until the length of path1.
 * @note 2. Path1 should be shorter than path2.
 * @return True if path1 is a prefix of path2, false otherwise.
 */
bool is_contained(const char* path1, const char* path2);

/**
 * @brief Finds a given string in a file.
 * @param target String to be found.
 * @param filepath Given file.
 * @returns True if found, false otherwise.
 */
bool find_string_in_file(const char *target, const char *filepath);

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

#endif // FILE_H