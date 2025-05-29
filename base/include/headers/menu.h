#ifndef MENU_H
#define MENU_H

/**
 * @file menu.h
 * @brief This file provides headers for functions regarding menus. 
 */

/**
 * @brief Prompts the user before clearing the configuration file.
 */
void clear_conf_prompt();

/**
 * @brief Prompts the user before exiting the program if it wants to clear the configuration file.
 */
void final_prompt();

/**
 * @brief Sub-menu for Discretionary Access Control options.
 */
void dac_menu();

/**
 * @brief Sub-menu for Access Control List(s) options.
 */
void acl_menu();

/**
 * @brief Sub-menu for Mandatory Access Control options.
 */
void mac_menu();

/**
 * @brief Sub-menu for logging options.
 */
void log_menu();

/**
 * @brief Sub-menu for auditing options.
 */
void acl_menu();

/**
 * @brief Sub-menu for firewall options.
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