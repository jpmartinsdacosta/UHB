#ifndef PERMS_H
#define PERMS_H

#include <stdbool.h>
#include <time.h>
#include "global_var.h"

/**
 * Generic struct memory allocation functions
 */

void* alloc_struct(size_t capacity, size_t size);

void* realloc_struct(void *structure, size_t new_capacity, size_t size);

/**
 * DACStruct functions
 */

/**
 * @brief Initializes the array of DACStruct.
 * @return True if successful, false otherwise.
 */
bool init_dac_array();

/**
 * @brief Clears the array of DACStruct.
 */
void clear_dac_array();

/**
 * @brief Retrieves data of a given element of DACStruct.
 * @param dac_index Index of the given DACStruct.
 */
void get_dac_data(size_t dac_index);

/**
 * @brief Adds an element to the DACStruct array.
 * @param Filepath Filepath where the permissions are being set.
 * @param user User to be added.
 * @param group Group to be added.
 * @param dac DAC permissions to be added.
 * @note THE RECURSIVE OPTION HAS NOT BEEN IMPLEMENTED YET.
 * @return True if successful, false otherwise.
 */
bool add_dac_element(const char *filepath, const char *user, const char *group, const char *dac);

/**
 * @brief Removes the last element from the DACStruct array.
 * @return True if successful, false otherwise.
 */
bool rem_dac_element();

/**
 * @brief Checks if a given filepath is already present in the DACStruct array.
 * @param filepath Filepath to be searched for.
 * @return True if the filepath exists, false otherwise.
 * @note 1. Needed when checking if another DACStruct element has to be created or not.
 * @note 2. THE RECURSIVE OPTION HAS NOT BEEN IMPLEMENTED YET.
 */
bool dac_filepath_exists(const char *filepath);

/**
 * ACLStruct functions
 */

/**
 * @brief Retrieves the ACLStruct data of a given element of DACStruct.
 * @param dac_index Index of the given DACStruct.
 * @param acl_index Index of the given ACLStruct.
 */
void get_acl_data(size_t dac_index, size_t acl_index);

/**
 * @brief Adds an element to the ACLStruct array of a given element of DACStruct.
 * @param dac_index Index of the given DACStruct.
 * @param fs Filesystem to be added.
 * @param acl ACL permissions to be added.
 * @note THE RECURSIVE OPTION HAS NOT BEEN IMPLEMENTED YET.
 * @return True if successful, false otherwise.
 */
bool add_acl_element(size_t dac_index, const char *fs, const char *acl);

/**
 * @brief Removes the last element from the ACLStruct array.
 * @param dac_index Index of the given DACStruct.
 * @param acl_index Index of the given ACLStruct.
 * @return True if successful, false otherwise.
 */
bool rem_acl_element(size_t dac_index, size_t acl_index);

/**
 * MACStruct functions
 */

 /**
 * @brief Retrieves the MACStruct data of a given element of DACStruct.
 * @param dac_index Index of the given DACStruct.
 * @param acl_index Index of the given MACStruct.
 */
void get_mac_data(size_t dac_index, size_t mac_index);

/**
 * @brief Adds an element to the MACStruct array of a given element of DACStruct.
 * @param dac_index Index of the given DACStruct.
 * @param mac MAC permissions to be added.
 * @note THE RECURSIVE OPTION HAS NOT BEEN IMPLEMENTED YET.
 * @return True if successful, false otherwise.
 */
bool add_mac_element(size_t dac_index, const char *mac);

/**
 * @brief Removes the last element from the MACStruct array.
 * @param dac_index Index of the given DACStruct.
 * @param mac_index Index of the given MACStruct.
 * @return True if successful, false otherwise.
 */
bool rem_mac_element(size_t dac_index, size_t mac_index);

/**
 * Policy-checking functions
 */

#endif // PERMS_H
