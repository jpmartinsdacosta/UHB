#ifndef CONFIG_H
#define CONFIG_H

/**
 * @brief Functions regarding the UHB configuration file.
 */

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
  * @brief Save the configuration to the default path.
  * The default path is "../config/uhb_config.txt".
  * @return 0 if the configuration was saved successfully, -1 otherwise.
  */

int save_config_default();

/**
* @brief Save the configuration to a custom path.
* @param path The path to save the configuration file.
* @return 0 if the configuration was saved successfully, -1 otherwise.
*/

int save_config_custom(char *path);

#endif // CONFIG_H