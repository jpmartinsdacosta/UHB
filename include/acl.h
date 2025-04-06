#ifndef ACL_H
#define ACL_H

/**
 * @file acl.h
 * @brief This file provides headers and additional data regarding Access Control Lists in UHB.
 */

/**
 * @brief Gets the current ACL settings of a file.
 */
void get_acl();

/**
 * @brief Checks if the filesystem of a given file is compatible with ACLs.
 * @param filepath The path to the file to check.
 * @return true if the filesystem is compatible with ACLs, false otherwise.
 */
bool acl_incompatible_fs(char *filepath);

#endif // ACL_H