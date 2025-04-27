#include <stdbool.h>

#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * @brief This file provides headers for functions regarding the configuration file.
 */

/**
 * @brief Returns whether the configuration file was modified or not.
 * @return true if modified, false otherwise.
 * @note Needed for the final_prompt() function.
 */
bool is_config_modified();

/**
 * @brief Sets the configuration file to an initial state.
 * This function is useful to overrite the content from the configuration file
 * to a default state.
 * 
 * The function also writes the operating system detected by the program.
 * @return true if the configuration file was created successfully, false otherwise.
 */
bool reset_config();

/**
 * @brief Check if the configuration file exists.
 * If the file does not exist, create it, otherwise print an error message.
 */
void config_exists();

/**
 * @brief Add a command to the configuration file.
 * @param command The command to be added to the configuration file.
 * @return true if the command was added successfully, false otherwise.
 */
bool add_config_command(const char *command);

/**
 * @brief View the configuration file.
 * This function will print the content of the configuration file.
 * If the file does not exist, it will print an error message.
 */
void view_config();

/**
 * @brief Function to be used by the user to find the first instance of a target in the configuration file. 
 * @note This is a test function that will be deprecated in the future.
 */
void user_find_first_config();

/**
 * @brief This function will apply the configuration file to the system.
 * @return true if the configuration file was applied successfully, false otherwise.
 * 
 * @note This function checks if uhb_os = XXX is in line 3 to ensure that the configuration file is being applied correctly.
 * Otherwise, it could execute the configuration file in the wrong operating system if a file has the same name as "uhb_os = XXX".
 */
bool apply_config();

#endif // CONFIG_H