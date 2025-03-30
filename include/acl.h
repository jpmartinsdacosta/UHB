#ifndef ACL_H
#define ACL_H

/**
 * @file acl.h
 * @brief This file provides headers and additional data regarding Access Control Lists in UHB.
 */

/**
 * @brief Checks in fstab whether the filesystem is compatible with Access Control Lists.
 * @param filepath The filepath to be checked.
 * @return True if compatible, false otherwise.
 * @note Incompatible filesystems are declared at incompatible_acl_fs in acl.c.
 */
bool acl_incompatible_fs(char *filepath);

/**
 * @brief Gets the current Access Control List(s) settings of a filesystem.
 */
void get_acl_common();



#endif // ACL_H