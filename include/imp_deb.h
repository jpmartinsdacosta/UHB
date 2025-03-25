#include <stdbool.h>

#ifndef IMP_DEB_H
#define IMP_DEB_H

/**
 *  @file imp_deb.h
 *  @brief This file provides headers for functions specific to Debian.
 *  @note This file is compiled when the operating system is detected as Debian.
 */

/**
 * @brief Check if supported executables exist on the Debian OS.
 * @param exec The array to store the results of the check.
 * 
 */
void exec_exists_deb(bool exec[4]);

#endif // IMP_DEB_H