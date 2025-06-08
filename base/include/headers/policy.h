#ifndef POLICY_H
#define POLICY_H

#include <stdbool.h>
#include <time.h>
#include "global_var.h"

/**
 * Initializes a FlagCollection with the specified number of flags.
 * Allocates memory and marks all flags as unused.
 * 
 * @param fc Pointer to FlagCollection to initialize.
 * @param numFlags Number of flags to allocate space for.
 */
void init_fc(FlagCollection *fc, int numFlags);

/**
 * Frees the memory allocated for the flags in the FlagCollection.
 * Resets the count to zero.
 * 
 * @param fc Pointer to FlagCollection to free.
 */
void free_fc(FlagCollection *fc);

/**
 * Sets the flag characters in the FlagCollection based on the given array.
 * 
 * @param fc Pointer to FlagCollection to set flags in.
 * @param flagArray Array of flag characters.
 */
void set_flags(FlagCollection *fc, const char *flagArray);

/**
 * Resets the 'used' status of all flags in the collection to false.
 * 
 * @param fc Pointer to FlagCollection to reset.
 */
void reset_flag_used(FlagCollection *fc);

/**
 * Prints all available flag options stored in the FlagCollection.
 * 
 * @param fc Pointer to FlagCollection whose flags will be printed.
 */
void print_flags(FlagCollection *fc);

/**
 * Initializes the FlagCollection with flags and prints them.
 * Combines init_fc, set_flags, and print_flags calls.
 * 
 * @param fc Pointer to FlagCollection to initialize.
 * @param numFlags Number of flags.
 * @param flagArray Array of flag characters.
 */
void init_flag(FlagCollection *fc, int numFlags, const char *flagArray);

/**
 * Searches for a flag character in the FlagCollection.
 * Marks the flag as used if found and unused.
 * Returns false on duplicate or incompatible flags.
 * 
 * @param flag Character to search for.
 * @param fc Pointer to FlagCollection to search.
 * @return true if flag found and unused, false otherwise.
 */
bool find_flag(char flag, FlagCollection *fc);

/**
 * Validates a flag command string against the allowed flags.
 * Checks for leading '-' and ensures all flags are recognized and not duplicated.
 * Resets used flags after checking.
 * 
 * @param command Flag command string (e.g., "-abc").
 * @param fc Pointer to FlagCollection with allowed flags.
 * @return true if all flags are valid and unique, false otherwise.
 */
bool check_flags(char *command, FlagCollection *fc);

/**
 * Allocates zero-initialized memory for an array of structures.
 * 
 * @param capacity Number of elements to allocate.
 * @param element_size Size of each element in bytes.
 * @return Pointer to allocated memory, or NULL on failure.
 */
void* alloc_struct(size_t capacity, size_t element_size);

/**
 * Reallocates memory for an array of structures to a new capacity.
 * Frees the memory if new_capacity is zero.
 * 
 * @param structure Pointer to current memory block.
 * @param new_capacity New number of elements.
 * @param element_size Size of each element in bytes.
 * @return Pointer to reallocated memory, or NULL if freed or failure.
 */
void* realloc_struct(void *structure, size_t new_capacity, size_t element_size);

/**
 * Initializes the DACStruct array with initial capacity.
 * 
 * @return true if successful, false otherwise.
 */
bool init_dac_array();

/**
 * Frees and clears the DACStruct array and resets size and capacity.
 */
void clear_dac_array();

/**
 * Prints the contents of a DACStruct element at the given index.
 * 
 * @param i Index of the DACStruct element.
 */
void get_dac_data(size_t i);

/**
 * Checks if a given filepath is contained within any recursive DAC entry.
 * 
 * @param fp Filepath to check.
 * @return true if contained, false otherwise.
 */
bool is_dac_contained(const char *fp);

/**
 * Adds a new DACStruct element to the array, reallocating if needed.
 * Copies input data and sets timestamp.
 * 
 * @param fp Filepath string.
 * @param user User string.
 * @param group Group string.
 * @param dac DAC permission string.
 * @param recursive Whether entry applies recursively.
 * @return true if added successfully, false otherwise.
 */
bool add_dac_element(const char *fp, const char *user, const char *group, const char *dac, bool recursive);

/**
 * Removes the most recent DACStruct element from the array.
 * Frees memory if array becomes empty.
 * 
 * @return true on success, false on failure.
 */
bool rem_dac_element();

/**
 * Finds the index of a DACStruct element by filepath.
 * 
 * @param fp Filepath string to search.
 * @return Index if found, -1 if not found.
 */
int find_dac_index_by_filepath(const char *fp);

/**
 * Initializes the ACLStruct array with initial capacity.
 * 
 * @return true if successful, false otherwise.
 */
bool init_acl_array();

/**
 * Frees and clears the ACLStruct array and resets size and capacity.
 */
void clear_acl_array();

/**
 * Prints the contents of an ACLStruct element at the given index.
 * 
 * @param i Index of the ACLStruct element.
 */
void get_acl_data(size_t i);

/**
 * Adds a new ACLStruct element to the array, reallocating if needed.
 * Copies input data and sets timestamp.
 * 
 * @param flag Flag string.
 * @param entry Entry string.
 * @param fp Filepath string.
 * @param is_recursive Boolean.
 * @return true if added successfully, false otherwise.
 */
bool add_acl_element(const char *flag, const char *entry, const char *fp, bool is_recursive);

/**
 * Removes the most recent ACLStruct element from the array.
 * Reallocates memory to shrink the array.
 * 
 * @return true on success, false on failure.
 */
bool rem_acl_element();

/**
 * Finds the index of an ACLStruct element by filepath.
 * 
 * @param fp Filepath string to search.
 * @return Index if found, -1 if not found.
 */
int find_acl_index_by_filepath(const char *fp);

/**
 * Initializes the MACStruct array with initial capacity.
 * 
 * @return true if successful, false otherwise.
 */
bool init_mac_array();

/**
 * Frees and clears the MACStruct array and resets size and capacity.
 */
void clear_mac_array();

/**
 * Prints the contents of a MACStruct element at the given index.
 * 
 * @param i Index of the MACStruct element.
 */
void get_mac_data(size_t i);

/**
 * Adds a new MACStruct element to the array, reallocating if needed.
 * Copies input data and sets timestamp.
 * 
 * @param fp Filepath string.
 * @param input Input string.
 * @param subject Subject string.
 * @param uid UID string.
 * @param gid GID string.
 * @param object Object string.
 * @param type Type string.
 * @param mode Mode string.
 * @return true if added successfully, false otherwise.
 */
bool add_mac_element(const char *fp, const char *input, const char *subject, const char *uid, const char *gid, const char *object, const char *type, const char *mode);

/**
 * Removes the most recent MACStruct element from the array.
 * Reallocates memory to shrink the array.
 * 
 * @return true on success, false on failure.
 */
bool rem_mac_element();

/**
 * Finds the index of a MACStruct element by filepath.
 * 
 * @param fp Filepath string to search.
 * @return Index if found, -1 if not found.
 */
int find_mac_index_by_filepath(const char *fp);

/**
 * Initializes all policy-related arrays (DAC, ACL, MAC).
 */
void init_all_arrays();

/**
 * Clears all policy-related arrays (DAC, ACL, MAC).
 */
void clear_all_arrays();


#endif // POLICY_H
