#ifndef SO_UTILS_H
#define SO_UTILS_H

/**
 * @brief Utility functions for UHB that are common for all operating systems.
 * 
 */

/**
 * @brief Check if a file exists.
 * return true if the file exists, false otherwise.
 */

bool path_exists(char *path);

/**
 * @brief Get the DAC of a file.
 * Common implementation for all operating systems.
 */

void get_dac_common();

#endif // UTILS_H