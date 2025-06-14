#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "os_interface.h"

#ifndef MODULE_VAR_H
#define MODULE_VAR_H

/**
 *  @file module_var.h
 *  @brief This file provides headers for functions specific to BSD.
 *  @note This file is compiled when the operating system is detected as BSD.
 *  All functions that are used in other source files MUST have the same names, otherwise compilation will fail.
 *  All functions that are only used inside this file MUST end with "_bsd".
 */

#define MODULE_PATH "../../additions/bsd/module_config.txt"
#define RC_CONF "/etc/rc.conf"

/**
 * @brief Checks if a given service is enabled in the rc.conf file.
 * @param service Service to be checked.
 * @returns True if enabled, false if disabled or not present.
 */
bool enabled_in_rc_conf(const char *service);

#endif // MODULE_VAR_H