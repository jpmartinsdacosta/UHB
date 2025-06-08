#include <stdbool.h>

#ifndef ACL_H
#define ACL_H

/**
 * @file acl.h
 * @brief File responsible for declaring headers of ACL functions common to all UHB Additions Modules.
 */

/**
 * @brief Checks if an ACL service is present in the system.
 * @return True if detected, false otherwise.
 */
bool acl_exists();

/**
 * @brief Gets the current ACL settings of a file.
 * @return true if successful, false otherwise.
 */
bool get_acl();

/**
 * @brief Sets the current ACL settings of a file.
 * @return true if successful, false otherwise.
 */
bool set_acl();

void rem_acl_rule();

void view_acl_configuration();

void reset_acl_configuration();

void apply_acl_configuration();

#endif // ACL_H