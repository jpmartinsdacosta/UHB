#ifndef INTERFACE_H
#define INTERFACE_H

/**
 * @file os_interface.h
 * @brief Common interface header for os-specific files.
 */

/**
 * @brief Returns the name of the current OS in a format understood by the config file.
 * @note The format is always "##uhb_os = XXX" with "XXX" being a 3-letter OS code.
 */
char* get_os();

#endif // OS_INTERFACE_H