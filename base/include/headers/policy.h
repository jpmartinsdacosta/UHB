#ifndef POLICY_H
#define POLICY_H

#include <stdbool.h>
#include <time.h>
#include "global_var.h"

/**
 * Flag management functions
 */

/**
 * @brief Initializes a FlagCollection.
 * @param fc The FlagCollection to initialize.
 * @param numFlags The number of flags to support.
 * @note DO NOT USE THIS FUNCTION, USE init_flag() INSTEAD!
 */
void init_fc(FlagCollection *fc, int numFlags);

/**
 * @brief Frees the memory allocated in a FlagCollection.
 * @param fc The FlagCollection structure.
 * @note THIS FUNCTION IS ALREADY USED IN check_flags()!
 */
void free_fc(FlagCollection *fc);

/**
 * @brief Set the flags of a FlagCollection from a predefined array.
 * @param fc The FlagCollection structure.
 * @param flagArray The predefined array.
 * @note DO NOT USE THIS FUNCTION, USE init_flag() INSTEAD!
 */
void set_flags(FlagCollection *fc, const char *flagArray);

/**
 * @brief Resets all the marked flags to false.
 * @param fc The FlagCollection structure.
 * @note THIS FUNCTION IS ALREADY USED IN check_flags()!
 */
void reset_flag_used(FlagCollection *fc);

/**
 * @brief Prints the available flags of a FlagCollection to the user.
 * @param fc The FlagCollection structure.
 * @note THIS FUNCTION IS ALREADY USED IN check_flags()!
 */
void print_flags(FlagCollection *fc);

/**
 * @brief Initializes and sets the flags to a FlagCollection to be used for an executable.
 * @param fc The FlagCollection structure.
 * @param numFlags The number of available flags in the flagArray
 * @param flagArray The list of valid flags for an executable.
 */
void init_flag(FlagCollection *fc, int numFlags, const char *flagArray);

/**
 * @brief Find a given flag inside a FlagCollection and mark it.
 * @param flag Flag to be found.
 * @param fc The FlagCollection structure.
 * @returns True if found and marked, false otherwise.
 * @note THIS FUNCTION IS ALREADY USED IN check_flags()!
 */
bool find_flag(char flag, FlagCollection *fc);

/**
 * @brief Checks if the flags given by the user for a specific executable are correct.
 * @param command The 'flags' part of the user input to be checked
 * @param fc The FlagCollection which contains the valid available flags for the executable.
 * @returns True if correct, false if the input is invalid.
 */
bool check_flags(char *command, FlagCollection *fc);

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
 * @param recursive If a recursive flag is present.
 * @return True if successful, false otherwise.
 */
bool add_dac_element(const char *filepath, const char *user, const char *group, const char *dac, bool recursive);

/**
 * @brief Removes the last element from the DACStruct array.
 * @return True if successful, false otherwise.
 */
bool rem_dac_element();

/**
 * @brief Searches for a DACStruct entry by exact filepath match.
 * @returns The index if found, otherwise -1.
 */
int find_dac_index_by_filepath(const char *filepath);

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
 * @param filepath Filepath to check whether a new instance of DACStruct needs to
 * be created or not when adding the new ACLStruct.
 * @param acl ACL permissions to be added.
 * @note THE RECURSIVE OPTION HAS NOT BEEN IMPLEMENTED YET.
 * @return True if successful, false otherwise.
 */
bool add_acl_element(const char *filepath, const char *acl);

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
 * @param filepath Filepath to check whether a new instance of DACStruct needs to
 * be created or not when adding the new MACStruct.
 * @param mac MAC permissions to be added.
 * @note THE RECURSIVE OPTION HAS NOT BEEN IMPLEMENTED YET.
 * @return True if successful, false otherwise.
 */
bool add_mac_element(const char *filepath, const char *mac);

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

#endif // POLICY_H
