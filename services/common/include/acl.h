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
 * @brief Checks if the filesystem of a given file is compatible with ACLs.
 * @param fp The path to the file to check.
 * @return true if the filesystem is compatible with ACLs, false otherwise.
 */
bool acl_incompatible_fs(char *fp);

#endif // ACL_H