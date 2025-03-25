#include <stdbool.h>

#ifndef SO_UTILS_H
#define SO_UTILS_H

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
bool sanitize_name(char *input);

/**
 * @brief Sanitizes options filtering out non-alphanumeric characters and the hyphen,
 * to avoid command injection when adding options for filesystem management.
 * @param input The user's input.
 * @return true if the input is sanitized, false otherwise.
 * 
 * ATTENTION:
 * THIS FUNCTION SHOULD ONLY BE USED WHEN ADDING FLAGS WHEN EXECUTING COMMANDS.
 */
bool sanitize_options(char *input);

/**
 * @brief Check if a file exists.
 * @param path The path to the file.
 * @return true if the file exists, false otherwise.
 * 
 * NOTE: The path_exists function does not have a function to sanitize the path.
 * This is due to the fact that too many types of characters are allowed in a path.
 * 
 * Thus, it is the responsibility of the user NOT to input any malicious paths.
 * Considering that the user is the system administrator, it is assumed that the user
 * knows what they are doing. ~João
 */
bool path_exists(char *path);

/**
 * @brief Check if the rc.local file exists.
 * If it exists, it updates the global rc_local variable in utils.c to true.
 */
void rc_local_exists_common();

/**
 * @brief Get the DAC of a file.
 * This function gets the DAC of a file by executing "ls -l --" on the file.
 * @return true if the file exists, false otherwise.
 */
bool get_dac_common();

/**
 * @brief Checks if the given permission is in a valid octal format.
 * @param permission The permission to check.
 * @return true if the permission is valid, false otherwise.
 */
bool check_permission(char *permission);

/**
 * @brief Check if the user exists.
 * @param user The user to check.
 * @return true if the user exists, false otherwise.
 */
bool check_user_common(char *user);

/**
 * @brief Check if the group exists.
 * @param group The group to check.
 * @return true if the group exists, false otherwise.
 */
bool check_group_common(char *group);

/**
 * @brief Set the DAC of a file.
 * This function sets the DAC of a file by executing "chmod" and "chown" on the file.
 * It also adds the commands to the configuration file.
 * @return true if the DAC is set, false otherwise.
 */
bool set_dac_common();

#endif // UTILS_H