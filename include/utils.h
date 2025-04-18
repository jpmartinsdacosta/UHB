#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H

/**
 * @file utils.h
 * @brief This file provides uility headers for functions in UHB that are common for all operating systems.
 */

/**
 * @brief Detect the operating system using preprocessor directives.
 * @return int -1 if the OS is Windows/unknown, 0 if FreeBSD, 1 if Linux.
 * Needed to send an integer to the show_menu function to determine which options to show.
 */
int os_detect();

/**
 * @brief Sanitizes user and group names, filtering out non-alphanumeric characters,
 * to avoid command injection during user/group searches.
 * @param input The user's input.
 * @return true if the input is sanitized, false otherwise.
 * 
 * ATTENTION:
 * THIS FUNCTION SHOULD ONLY BE USED FOR USER OR GROUP NAMES.
 */
bool sanitize_name(const char *input);

/**
 * @brief Sanitizes options filtering out non-alphanumeric characters and the hyphen,
 * to avoid command injection when adding options for filesystem management.
 * @param input The user's input.
 * @return true if the input is sanitized, false otherwise.
 * 
 * ATTENTION:
 * THIS FUNCTION SHOULD ONLY BE USED WHEN ADDING FLAGS WHEN EXECUTING COMMANDS.
 */
bool sanitize_options(const char *input);

/**
 * @brief Checks if the given permission is in a valid octal format.
 * @param permission The permission to check.
 * @return true if the permission is valid, false otherwise.
 */
bool check_permission(const char *permission);

/**
 * @brief Check if the user exists.
 * @param user The user to check.
 * @return true if the user exists, false otherwise.
 */
bool check_user(const char *user);

/**
 * @brief Check if the group exists.
 * @param group The group to check.
 * @return true if the group exists, false otherwise.
 */
bool check_group(const char *group);

#endif // UTILS_H