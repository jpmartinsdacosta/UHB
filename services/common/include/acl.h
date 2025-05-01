#ifndef ACL_H
#define ACL_H

/**
 * Functions specific to Access Control Lists:
 */

/**
 * @brief Gets the current ACL settings of a file.
 * @return true if successful, false otherwise.
 */
bool get_acl();

/**
 * @brief Check if Access Control Lists are enabled in a given filesystem in FreeBSD.
 * @param filesystem Filesystem provided by the user.
 * @note This function is necessary in BSD due to the need for a fstab entry.
 */
bool is_acl_enabled_bsd(const char* fp);

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

#endif // ACL_H