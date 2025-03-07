#ifndef SO_UTILS_H
#define SO_UTILS_H

/**
 * @brief Utility functions for UHB that are common for all operating systems.
 * 
 */

/**
 * @brief Detect the operating system using uname -s.
 * 
 * This function attempts to detect the operating system by executing the 
 * 'uname -s' command. It returns an integer representing the detected OS.
 * 
 * @return int -1 if the OS is unknown, 0 if FreeBSD, 1 if Linux.
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
 * @brief Check if the configuration file exists.
 * If the configuration file does not exist, the file is created. 
 * Otherwise, the function does nothing.
 */

void config_exists();

/**
 * @brief Get the user's input for a path.
 * @param path The path to store the user's input.
 */

void get_user_input(char *path);

/**
 * @brief Test user command if it works before adding it to the configuration file.
 * @param command The command to test.
 * @return int 0 if the command works, -1 otherwise.
 */

int test_user_command(char *command);

/**
 * @brief Get the DAC of a file.
 * This function gets the DAC of a file by executing "ls -l --" on the file.
 */

void get_dac_common();



/**
 * @brief Show the menu to the user.
 * @param so The operating system detected by so_detect.
 * @param exec The value retuned by exec_exists_common.
 * This function shows the menu to the user, based on the operating system detected.
 */
void show_menu();

#endif // UTILS_H