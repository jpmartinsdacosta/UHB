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
 * @param option The array to store the results of the check.
 * 
 */

void exec_exists_deb(int option[4]);

#endif // IMP_DEB_H