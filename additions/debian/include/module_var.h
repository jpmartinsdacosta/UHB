#include <stdbool.h>
#include "os_interface.h"

#ifndef MODULE_VAR_H
#define MODULE_VAR_H

/**
 *  @file module_var.h
 *  @brief This file provides headers for functions specific to Debian.
 *  @note This file is compiled when the operating system is detected as Debian.
 *  All functions that are used in other source files MUST have the same names, otherwise compilation will fail.
 *  All functions that are only used inside this file MUST end with "_deb".
 */

#define MODULE_PATH "../../additions/debian/module_config.txt"

#endif // MODULE_VAR_H