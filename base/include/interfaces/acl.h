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
 * @brief Checks if the filesystem of a given file is compatible with ACLs.
 * @param fp The path to the file to check.
 * @return true if the filesystem is compatible with ACLs, false otherwise.
 */
bool acl_incompatible_fs(char *fp);

/**
 * @brief Sets the current ACL settings of a file.
 * @return true if successful, false otherwise.
 */
bool set_acl();

void view_acl_configuration();

#endif // ACL_H