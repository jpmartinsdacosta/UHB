#ifndef MENU_H
#define MENU_H

/**
 * @file menu.h
 * @brief This file provides headers for functions regarding menus. 
 */

/**
 * @brief Check if an executable exists.
 * 
 * This function checks from an array of executables if they exist in the system,
 * using "which -s" to check if the executable exists. It modifies exec array
 * accordingly in menu.c to 0 or 1 if the executable exists.
 */
 void exec_exists_common();

/**
 * @brief Function that returns the size of a menu.
 * @param options The const char menu options declared at the start of menu.c
 * @returns the size of the menu.
 */
int get_diccionary_size(const char **options);

/**
 * @brief Prompts the user before clearing the configuration file.
 */
void clear_conf_prompt();

/**
 * @brief Prompts the user before exiting the program if it wants to clear the configuration file.
 */
void final_prompt();

/**
 * @brief Function the displays a menu, whose size should not be greater than 10 options.
 * @param prompt The menu title
 * @param options The const char menu options declared at the start of menu.c
 * @return -1 if error, selected option otherwise. 
 */
int display_menu(const char *prompt, const char **options);

/**
 * @brief Sub-menu for Discretionary Access Control options.
 */
void dac_menu();

/**
 * @brief Sub-menu for Access Control List(s) options.
 */
void acl_menu();

/**
 * @brief Sub-menu for configuration file options.
 */
void conf_menu();

/**
 * @brief Main menu.
 */
void main_menu();

 #endif // MENU_H