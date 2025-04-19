#include <stdbool.h>
#include <time.h>

#ifndef PERMS_H
#define PERMS_H

/**
 * @file perms.h
 * @brief This file provides headers for functions regarding general UHB permissions.
 * @note Permissions are made of DAC, ACL and MAC policy. Specific implementations of ACL and MAC can be found in the services folder.
 */

/**
 * Functions common to both DAC and ACL
 */

/**
 * @brief Generic function that allocates memory for dynamic structures.
 * @param capacity Current capacity of the struct.
 * @param element_size Current size of one of the elements of the struct.
 */
void* alloc_struct(size_t capacity, size_t element_size);

/**
 * @brief Generic function that rellocates memory for dynamic structures.
 * @param struct_array Pointer to the struct whose memory is being reallocated.
 * @param capacity Current capacity of the struct.
 * @param element_size Current size of one of the elements of the struct.
 */
void* realloc_struct(void *struct_array, size_t capacity, size_t element_size);

/**
 * @brief Prints the timestamp in a human readable format.
 * @param timestamp The current element's timestamp.
 */
void print_timestamp(time_t timestamp);

/**
 * Functions regarding DAC
 */

/**
 * @brief Function that returns whether DACStruct is empty.
 * @returns True if empty, false otherwise.
 */
bool is_dac_empty();

/**
 * @brief Function that initializes DACStruct.
 * @returns True if successful, false otherwise.
 * @note DACStruct is initialized by changing the value of dac_capacity to 1.
 */
bool init_dac();

/**
 * @brief Clears ALL data in DACStruct.
 */
void clear_dac();

/**
 * @brief Adds a timestamp to a DACStruct element.
 * @param index The index of the DACStruct element.
 * @returns True if successful, false otherwise.
 */
bool add_dac_time(int index);

/**
 * @brief Adds/removes elements from DACStruct.
 * @param add If true, adds elements to the struct, if false, removes elements from the struct.
 * @returns True if successful, false otherwise.
 */
bool mod_dac_element(bool add);

/**
 * @brief Adds data to a DACStruct element.
 * @param fp Filepath to be added.
 * @param user User to be added.
 * @param group Group to be added.
 * @param dac DAC permissions to be added.
 * @returns True if successful, false otherwise. 
 */
bool add_dac_data(char *fp, char *user, char *group, char *dac);

/**
 * @brief Removes the last element of DACStruct.
 * @returns True if successful, false otherwise.
 */
bool rem_dac_data();

/**
 * @brief Prints the data of a DACStruct.
 * @param index The current index.
 * @note To be used in a for loop using array indexes.
 */
void get_dac_data(int index);

/**
 * Functions regarding ACLs
 */

/**
 * @brief Function that returns whether ACLStruct is empty.
 * @returns True if empty, false otherwise.
 */
bool is_acl_empty();

/**
 * @brief Function that initializes ACLStruct.
 * @returns True if successful, false otherwise.
 * @note ACLStruct is initialized by changing the value of acl_capacity to 1.
 */
bool init_acl();

/**
 * @brief Clears ALL data in ACLStruct.
 */
void clear_acl();

/**
 * @brief Adds a timestamp to a ACLStruct element.
 * @returns True if successful, false otherwise.
 */
bool add_acl_time();

/**
 * @brief Adds/removes elements from ACLStruct.
 * @param add If true, adds elements to the struct, if false, removes elements from the struct.
 * @returns True if successful, false otherwise.
 */
bool mod_acl_element(bool add);

/**
 * @brief Adds data to a ACLStruct element.
 * @param fp Filepath to be added.
 * @param fs Filesystem to be added.
 * @returns True if successful, false otherwise. 
 */
bool add_acl_data(char *fp, char *fs);

/**
 * @brief Removes the data of an ACLStruct element.
 * @returns True if successful, false otherwise. 
 */
bool rem_acl_data();

/**
 * @brief Prints the data of a ACLStruct element.
 * @param index The current index.
 * @note To be used in a for loop using array indexes.
 */
void get_acl_data(int index);

#endif // PERMS_H