#ifndef MAC_H
#define MAC_H

/**
 * @file mac.h
 * @brief File responsible for declaring headers of MAC functions common to all UHB Additions Modules.
 */

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

#endif // MAC_H