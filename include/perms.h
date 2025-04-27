#include <stdbool.h>
#include <time.h>

#ifndef PERMS_H
#define PERMS_H

/**
 * @file perms.h
 * @brief This file provides headers for functions regarding general UHB permissions.
 * @note Permissions are added dynamically via UHBStruct and its DAC, ACL and MAC sub-structures.
 */

/**
 * Functions common to all
 */

/**
 * @brief Allocates memory for a dynamic structure.
 * @param capacity The current capacity of the dynamic structure.
 * @param size The current size of the dyanmic structure.
 */
void* alloc_struct(size_t capacity, size_t size);

/**
 * @brief Reallocates memory for a dynamic structure.
 * @param structure Pointer to the dynamic structure whose memory is being reallocated.
 * @param new_capacity The new capacity of the dynamic structure.
 * @param size The current size of the dyanmic structure.
 */
void* realloc_struct(void *structure, size_t new_capacity, size_t size);

/**
 * UHBStruct functions
 */

/**
 * @brief Initializes UHBStruct.
 * @return True if successful, false otherwise.
 */
bool init_uhb_array();

/**
 * @brief Initializes all the sub-dynamic structures inside a specific UHBStruct
 * @param fp Filepath
 * @return True if successful, false otherwise.
 */
bool init_uhb_element_array(const char *fp);

/**
 * @brief Clears all the sub-dynamic structures inside a specifc UHBStruct.
 */
void clear_uhb_element_array();

/**
 * @brief Clears UHBStruct.
 */
void clear_uhb_array();

/**
 * @brief Dynamically adds a UHBStruct structure to UHBStruct
 * @param fp Filepath
 * @return True if successful, false otherwise.
 */
bool add_uhb_element(const char *fp);

/**
 * @brief Shows the data of a given UHBStruct to the user.
 * @param index The index of the current UHBStruct to retrieve information.
 */
void get_uhb_data(int index);

/**
 * @brief Removes the topmost/latest UHBStruct structure from UHBStruct.
 * @return True if successful, false otherwise.
 */
bool rem_uhb_element();

/**
 * DACStruct functions
 */

/**
 * @brief Adds an element to DACStruct
 * @param user User to be added
 * @param group Group to be added
 * @param dac DAC permissions to be added
 * @return True if successful, false otherwise.
 */
bool add_dac_element(char *user, char *group, char *dac);

/**
 * @brief Shows the data of a given DACStruct to the user.
 * @param index The index of the current DACStruct to retrieve information.
 */
void get_dac_data(int index);

/**
 * @brief Removes the topmost/latest DACStruct structure from DACStruct.
 * @return True if successful, false otherwise.
 */
bool rem_dac_element();

/**
 * ACLStruct functions
 */

/**
 * @brief Adds an element to ACLStruct
 * @param fs Filesystem
 * @param acl ACL permissions to be added
 * @return True if successful, false otherwise.
 */
bool add_acl_element(char *fs, char *acl);

/**
 * @brief Shows the data of a given ACLStruct to the user.
 * @param index The index of the current ACLStruct to retrieve information.
 */
void get_acl_data(int index);

/**
 * @brief Removes the topmost/latest ACLStruct structure from ACLStruct.
 * @return True if successful, false otherwise.
 */
bool rem_acl_element();

/**
 * MACStruct functions
 */

/**
 * @brief Adds an element to MACStruct
 * @param fs Filesystem
 * @param mac MAC permissions to be added
 * @return True if successful, false otherwise.
 */
bool add_mac_element(char *mac);

/**
 * @brief Shows the data of a given MACStruct to the user.
 * @param index The index of the current MACStruct to retrieve information.
 */
void get_mac_data(int index);

/**
 * @brief Removes the topmost/latest MACStruct structure from MACStruct.
 * @return True if successful, false otherwise.
 */
bool rem_mac_element();

#endif // PERMS_H