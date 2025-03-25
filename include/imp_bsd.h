#include <stdbool.h>

#ifndef IMP_BSD_H
#define IMP_BSD_H

/**
 *  @file imp_bsd.h
 *  @brief This file provides headers for functions specific to FreeBSD.
 *  @note This file is compiled when the operating system is detected as FreeBSD.
 */

/**
 * @brief Check if supported executables exist on the FreeBSD OS.
 * @param exec The array to store the results of the check.
 * 
 */
void exec_exists_bsd(bool exec[4]);

#endif // IMP_BSD_H