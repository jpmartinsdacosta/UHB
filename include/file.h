#ifndef FILE_H
#define FILE_H

/**
 * @file file.h
 * @brief This file provides headers for functions regarding general file managment throughout the program.
 */

/**
 * @brief Finds the first instance of a string inside a file.
 * @param target the string to be found inside the file.
 * @param filepath the filepath of the file where to search.
 * @returns -1 if no instances were found or as an error result, returns the line position otherwise.
 */
int find_first_in_file(char *target, char *filepath);

/**
 * @brief Finds two strings inside a line of a file.
 * @param a the first string to be found inside the file.
 * @param b the second string to be found inside the file.
 * @param filepath the filepath of the file where to search.
 */
bool find_strings_in_line(char *a, char *b, char *filepath);

/**
 * @brief Finds the filesystem of a given path.
 * @param filepath the path to be checked.
 * @returns the filesystem of the given path if it exists, NULL otherwise.
 */
char* find_fs_from_path(char *filepath);

#endif // FILE_H