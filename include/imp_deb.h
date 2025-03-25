#include <stdbool.h>

#ifndef IMP_DEB_H
#define IMP_DEB_H

/**
 *  @file imp_bsd.h
 *  @brief The Debian implementation.
 * 
 *  This function is called when the operating system is detected as Debian.
 *  It provides UHB functions that are specific to the Debian operating system.
 * 
 */

/**
 * @brief Check if supported executables exist on the Debian OS.
 * @param exec The array to store the results of the check.
 * 
 */

void exec_exists_deb(bool exec[4]);

#endif // IMP_DEB_H