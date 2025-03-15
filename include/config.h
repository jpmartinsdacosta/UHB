#ifndef CONFIG_H
#define CONFIG_H

/**
 * @brief Functions regarding the UHB configuration file.
 */

/**
 * @brief Sets the configuration file to an initial state.
 * This function is useful to overrite the content from the configuration file
 * to a default state.
 * 
 * The function also writes the operating system detected by the program.
 * @return true if the configuration file was created successfully, false otherwise.
 */

 bool set_initial_config();

/**
 * @brief Check if the configuration file exists.
 * If the file does not exist, create it, otherwise print an error message.
 */

void config_exists();

/**
 * @brief Add a command to the configuration file.
 * @param command The command to be added to the configuration file.
 */

int add_config_command(char *command);

/**
 * @brief View the configuration file.
 * This function will print the content of the configuration file.
 * If the file does not exist, it will print an error message.
 */

void view_config();

#endif // CONFIG_H