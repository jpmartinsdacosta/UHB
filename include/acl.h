#ifndef ACL_H
#define ACL_H

/**
 * @file acl.h
 * @brief This file provides headers and additional data regarding Access Control Lists in UHB.
 */

/**
 * @brief Checks in fstab whether the filesystem is compatible with ACLs.
 * @param filepath The filepath to be checked.
 * @return True if compatible, false otherwise.
 * @note Incompatible filesystems are declared at incompatible_acl_fs in acl.c.
 */
bool acl_incompatible_fs(char *filepath);

/**
 * @brief Gets the current ACL settings of a file.
 */
//void get_acl_common();

/**
 * @brief Sets the ACL settings of a file.
 * @param os Current OS.
 * @return True if successful, false otherwise.
 * @note The implementation of this function in BSD needs to check if ACLs are enabled in fstab.
 */
//bool set_acl_common(int os);

#endif // ACL_H