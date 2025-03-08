#ifndef SO_UTILS_H
#define SO_UTILS_H

/**
 * @brief Utility functions for UHB that are common for all operating systems.
 * 
 */

/**
 * @brief Detect the operating system using preprocessor directives.
 * @return int -1 if the OS is Windows/unknown, 0 if FreeBSD, 1 if Linux.
 * 
 * Needed to send an integer to the show_menu function to determine which options to show.
 */

int so_detect();

/**
 * @brief Check if an executable exists.
 * @param so The operating system detected by so_detect.
 * 
 * This function checks from an array of executables if they exist in the system,
 * using "which -s" to check if the executable exists. It modifies option array
 * accordingly in utils.c to 0 or 1 if the executable exists.
 */

void exec_exists_common();

/**
 * @brief Check if a file exists.
 * @param path The path to the file.
 * @return true if the file exists, false otherwise.
 */

bool path_exists(char *path);

/**
 * @brief Check if the rc.local file exists.
 * If it exists, it updates the rc_local variable in utils.c to true.
 */

void rc_local_exists_common();

/**
 * @brief Get the user's input for a path.
 * @param path The path to store the user's input.
 * @return true if the user's input is valid, false otherwise.
 */

bool get_user_input(char *path);

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
 * @brief Check if the user/group exists.
 * @param target The user/group to check.
 * @return true if the user/group exists, false otherwise.
 */

bool check_ug_common(char *target);

/**
 * @brief Show the menu to the user.
 * @param so The operating system detected by so_detect.
 * @param exec The value retuned by exec_exists_common.
 * This function shows the menu to the user, based on the operating system detected.
 */
void show_menu();

#endif // UTILS_H