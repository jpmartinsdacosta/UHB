#include <stdbool.h>

#ifndef IMP_BSD_H
#define IMP_BSD_H

/**
 *  @file imp_bsd.h
 *  @brief The BSD implementation.
 * 
 *  This function is called when the operating system is detected as FreeBSD.
 *  It provides UHB functions that are specific to the FreeBSD operating system.
 * 
 */

/**
 * @brief Check if supported executables exist on the FreeBSD OS.
 * @param exec The array to store the results of the check.
 * 
 */

void exec_exists_bsd(bool exec[4]);

#endif // IMP_BSD_H