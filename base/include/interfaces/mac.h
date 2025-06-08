#ifndef MAC_H
#define MAC_H

/**
 * @file mac.h
 * @brief File responsible for declaring headers of MAC functions common to all UHB Additions Modules.
 */

/**
 * @brief Checks if an MAC service is present in the system.
 * @return True if detected, false otherwise.
 */
bool mac_exists();

int parse_mac_mode_flags(const char *mode_str);

/**
 * @brief Gets the current MAC settings of a file.
 * @returns True if successful, false otherwise.
 */
bool get_mac();

/**
 * @brief Sets the current MAC settings of a file.
 * @returns True if successful, false otherwise.
 */
bool set_mac();

void view_mac_manual();

void view_mac_configuration();

void reset_mac_configuration(bool load_from_backup);

void apply_mac_configuration();

void free_mac();

#endif // MAC_H