#include <stdbool.h>
#include <time.h>
#include "global_var.h"

#ifndef PERMS_H
#define PERMS_H

/**
 * @file perms.h
 * @brief This file provides headers for functions regarding general UHB permissions.
 * @note Permissions are added dynamically via UHBStruct and its DAC, ACL and MAC sub-structures.
 */

/**
 * Memory allocation functions
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
 * @brief Initalizes memory for an INSTANCE of UHBStruct
 * @param instance Instance to be initialized.
 * @param filepath Filepath to be added
 * @return True if successful, false otherwise.
 */
bool init_uhb(struct UHBStruct *instance, const char *filepath);

/**
 * @brief Clears memory for an INSTANCE of UHBStruct
 * @param instance Instance to be initialized.
 */
void clear_uhb(struct UHBStruct *instance);

/**
 * @brief Initalizes an ARRAY of UHBStruct
 * @return True if successful, false otherwise.
 */
bool init_uhb_array();

/**
 * @brief Clears an ARRAY of UHBStruct
 */
void clear_uhb_array();

/**
 * @brief Adds an INSTANCE of UHBStruct to the array
 * @param filepath Filepath to be added
 * @return True if successful, false otherwise.
 */
bool add_uhb_element(const char *filepath);

/**
 * @brief Removes an INSTANCE of UHBStruct to the array
 * @return True if successful, false otherwise.
 */
bool rem_uhb_element();

/**
 * @brief Checks if a given filepath is already present in an instance of UHBStruct.
 * @param filepath to be searched.
 * @note This function is essential to check whether a new instance of UHBStruct must be created.
 * @return True if found, false otherwise.
 */
bool uhb_filepath_exists(const char *filepath);

/**
 * DACStruct functions
 */

/**
 * @brief Adds an instance of DACStruct for a given UHBStruct
 * @param uhb_index Index of the given UHBStruct.
 * @param user User to be added.
 * @param group Group to be added.
 * @param dac DAC permissions to be added.
 * @return True if successful, false otherwise.
 */
bool add_dac_element(size_t uhb_index, char *user, char *group, char *dac);

/**
 * @brief Gets information of an instance of DACStruct for a given UHBStruct
 * @param uhb_index Index of the given UHBStruct.
 * @param dac_index Index of the DACStruct to be retrieved.
 */
void get_dac_data(size_t uhb_index, size_t dac_index);

/**
 * @brief Removes an instance of DACStruct for a given UHBStruct
 * @param uhb_index Index of the given UHBStruct.
 * @param dac_index Index of the DACStruct to be removed.
 * @return True if successful, false otherwise.
 */
bool rem_dac_element(size_t uhb_index);

#endif // PERMS_H