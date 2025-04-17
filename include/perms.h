#include <stdbool.h>

#ifndef SO_PERMS_H
#define SO_PERMS_H

/**
 * @file perms.h
 * @brief This file provides headers regarding DAC and ACL functions.
 */

 /**
 * Functions specific for UhbStruct
 */

 

/**
 * Functions specific for Discretionary Access Control (DAC):
 */

/**
 * @brief Get the DAC of a file.
 * This function gets the DAC of a file by executing "ls -l --" on the file.
 * @return true if the file exists, false otherwise.
 */
bool get_dac();

/**
 * @brief Set the DAC of a file.
 * This function sets the DAC of a file by executing "chmod" and "chown" on the file.
 * It also adds the commands to the configuration file.
 * @return true if the DAC is set, false otherwise.
 */
bool set_dac();

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

#endif // PERMS_H
