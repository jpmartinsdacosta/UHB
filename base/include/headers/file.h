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
 * @brief Finds the first instance of a string inside a file.
 * @param target the string to be found inside the file.
 * @param fp the fp of the file where to search.
 * @returns -1 if no instances were found or as an error result, returns the line position otherwise.
 */
int find_first_in_file(const char *target, const char *fp);

/**
 * @brief Finds two strings inside a line of a file.
 * @param a the first string to be found inside the file.
 * @param b the second string to be found inside the file.
 * @param fp the fp of the file where to search.
 */
bool find_strings_in_line(const char *a, const char *b, const char *fp);

/**
 * @brief Finds the filesystem of a given path.
 * @param fp the path to be checked.
 * @returns the filesystem of the given path if it exists, NULL otherwise.
 */
char* find_fs_from_path(const char *fp);

/**
 * @brief Copies a file, character by character to another.
 * @param source        Source fp to be copied.
 * @param destination   Destination fp to be copied to.
 * @returns True if successful, false otherwise.
 */
bool copy_file(const char *source, const char *destination);

/**
 * @brief Finds and replaces the first occurence of a given string in a file.
 * @param fp File in which to make replacements.
 * @param target String to be replaced.
 * @param replacement Replacment.
 * @return True if successful, false otherwise.
 */
bool find_first_and_replace(const char *fp, const char *target, const char *replacement);

/**
 * @brief Finds and replaces the first occurence of a given string in a file.
 * @param fp File in which to make replacements.
 * @param target String to be replaced.
 * @param replacement Replacment.
 * @return True if successful, false otherwise.
 */
bool find_n_and_replace(const char *fp, const char *target, const char *replacement, int num);

#endif // FILE_H