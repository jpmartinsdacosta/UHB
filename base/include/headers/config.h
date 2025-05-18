#include <stdbool.h>

#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * @brief This file provides headers for functions regarding the configuration of UHB.
 */

/**
 * Functions regarding the UHB Base Configuration file at uhb/base/config/files/config.sh
 */

/**
 * @brief Returns whether the configuration file was modified or not.
 * @return true if modified, false otherwise.
 * @note Needed for the final_prompt() function.
 */
bool is_conf_file_mod();

/**
 * @brief Resets the UHB Base configuration file.
 * The function also writes the operating system detected by the program.
 * @return True if the configuration file was created successfully, false otherwise.
 */
bool reset_uhb_conf();

/**
 * @brief Check if a configuration file exists.
 * If the file does not exist, create it, otherwise print an error message.
 */
void uhb_conf_exists(const char *filepath);

/**
 * Functions regarding the service configuration files at uhb/base/config/services/
 */

void detect_execs();

/**
 * @brief Resets ALL service configuration files.
 * @return True if successful, false otherwise.
 */
bool reset_service_conf();

/**
 * @brief Add a command to the a given service configuration file.
 * @param command The command to be added.
 * @param filepath The filepath of the given service configuration file.
 * @return True if the command was added successfully, false otherwise.
 */
bool add_service_command(const char *command, const char *filepath);

/**
 * Functions common to all
 */

/**
 * @brief Resets both the UHB Base configuration and all the service configuration files.
 * @return True if successful, false otherwise.
 * 
 */
bool reset_conf();

#endif // CONFIG_H