#ifndef SO_MENU_H
#define SO_MENU_H

/**
 * @brief Menu functions used throughout UHB. 
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
 * @brief Show the menu to the user.
 * This function shows the menu to the user, based on the operating system detected.
 */

 /**
  * @brief Function that returns the size of a menu.
  * @param options The const char menu options declared at the start of menu.c
  * @returns the size of the menu.
  */
  int get_menu_size(const char *options);

  /**
   * 
   */
  void final_prompt();

  /**
   * @brief Function the displays a menu, whose size should not be greater than 10 options.
   * @param prompt The menu title
   * @param options The const char menu options declared at the start of menu.c
   * @return -1 if error, selected option otherwise. 
   */
  int display_menu(char *prompt, const char *options);

  /**
   * 
   */
  void dac_menu();

  /**
   * 
   */
  void main_menu();

 #endif // MENU_H