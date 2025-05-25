#include <stdbool.h>
#include "global_var.h"

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

/**
 * @brief Checks if a given executable exists in the system.
 * @param exec The name of the executable to check.
 * @returns True if found, false otherwise.
 */
bool exec_exists(char *exec);

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
 * @brief Checks if one of the flags given by the user is recursive.
 * @param flags The flags added by the user.
 * @return True if recursive, false otherwise.
 */
bool is_recursive(const char *flags);

/**
 * @brief Checks if a port is open or not in the system.
 * @param port The port number to be checked.
 * @returns True if open, false if closed.
 */
bool is_port_open(int port);

#endif // UTILS_H