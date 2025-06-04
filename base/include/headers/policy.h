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

 bool init_dac_array();

 void clear_dac_array();

 void get_dac_data(size_t i);

 bool is_dac_contained(const char *fp);

 bool add_dac_element(const char *fp, const char *user, const char *group, const char *dac, bool recursive);

 bool rem_dac_element();

 int find_dac_index_by_filepath(const char *fp);

/**
 * ACLStruct functions
 */

 bool init_acl_array();

 void clear_acl_array();

 void get_acl_data(size_t i);

 bool add_acl_element(const char *fp, const char *acl);

 bool rem_acl_element(size_t i);

 int find_acl_index_by_filepath(const char *fp);

/**
 * MACStruct functions
 */

 bool init_mac_array();

 void clear_mac_array();

 void get_mac_data(size_t i);

 bool add_mac_element(const char *fp, const char *mac);

 bool rem_mac_element(size_t i);

 int find_mac_index_by_filepath(const char *fp);

/**
 * Common to all
 */

 void init_all_arrays();

 void clear_all_arrays();
/**
 * Policy-checking functions
 */

#endif // POLICY_H
