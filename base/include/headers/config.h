#include <stdbool.h>

#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * @brief This file provides headers for functions regarding the configuration of UHB.
 */

 /**
  * @brief Changes the value of the load_
  * If set to true, all service configuration files will be copied from the backups
  * folder instead from their original source files.
  */
void load_from_backup_function();

void show_debug_messages_function();

void reset_configuration_file();

void view_configuration_file();

/**
 * @brief Resets and clears current DAC, ACL and MAC configurations.
 */
void reset_file_service_policy();

/**
 * @brief Applies the current DAC, ACL and MAC configurations.
 */
void apply_file_service_policy();

/**
 * @brief Void function with the sole purpose of being the place where functions that
 * need to be executed during initialization.
 * @note This function should be declared once BEFORE the main_menu() function in main.c
 */
void initialize_uhb();

/**
 * @brief Void function with the sole purpose of being the place where functions that
 * need to be executed before termination.
 * @note This function should be declared once AFTER the main_menu() function in main.c.
 */
void terminate_uhb();

#endif // CONFIG_H