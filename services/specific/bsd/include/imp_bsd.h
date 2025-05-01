#include <stdbool.h>
#include "os_interface.h"

#ifndef IMP_BSD_H
#define IMP_BSD_H

/**
 *  @file imp_bsd.h
 *  @brief This file provides headers for functions specific to FreeBSD.
 *  @note This file is compiled when the operating system is detected as FreeBSD.
 *  All functions that are used in other source files MUST have the same names, otherwise compilation will fail.
 *  All functions that are only used inside this file MUST end with "_bsd".
 */

/**
 * @brief Check if supported executables exist on the FreeBSD OS.
 * @param exec The array to store the results of the check.
 * 
 */
void exec_exists(bool exec[4]);

#endif // IMP_BSD_H