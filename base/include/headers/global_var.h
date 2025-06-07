#include <time.h>
#include <stdbool.h>

#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

/**
 * @file global_vars.h
 * @brief Header file responsible for declaring extern global constants used throughout UHB.
 */

// Global constants
#define MAX_FILEPATH_SIZE 4096  // 4096 characters max. in Linux
#define MAX_OPTIONS_LENGTH 100  // 100, for the large ACL options list.
#define MAX_LINE_LENGTH 2048    // 2048, to be used for commands too.
#define MAX_NAME_LENGTH 32      // 32, standard.

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Current UHB base configuration file.
#define UHB_BASE_CONFIG_CURRENT     "../config/current/uhb.conf"

// Backup UHB base configuration file, to be copied to current when resetting.
#define UHB_BASE_CONFIG_BACKUP      "../config/backups/uhb.conf"

// Configuration template filepaths
#define UHB_SCRIPT_TEMPLATE_PATH    "../config/backups/script_template.txt"

/**
 * Structures to manage flags/options when executing each service
 */

typedef struct{
    char flag;
    bool used;
} Flag;

typedef struct{
    Flag *flags;
    int count;
} FlagCollection;

/**
 * Structures for the validation of DAC, ACL and MAC policy
 */

typedef struct {
    char acl[MAX_LINE_LENGTH];
    char fp[MAX_FILEPATH_SIZE];
    bool recursive;
    time_t timestamp;
} ACLStruct;

typedef struct {
    char fp[MAX_FILEPATH_SIZE];
    char input[MAX_LINE_LENGTH];
    char subject[MAX_NAME_LENGTH];
    char uid[MAX_NAME_LENGTH];
    char gid[MAX_NAME_LENGTH];
    char object[MAX_NAME_LENGTH];
    char type[MAX_LINE_LENGTH];
    char mode[MAX_LINE_LENGTH];
    bool recursive;
    time_t timestamp;
} MACStruct;

typedef struct {
    char fp[MAX_FILEPATH_SIZE];
    char user[MAX_NAME_LENGTH];
    char group[MAX_NAME_LENGTH];
    char dac[6];
    bool recursive;
    time_t timestamp;
} DACStruct;

// Global policy arrays
extern DACStruct *dac_array;
extern size_t dac_size, dac_capacity;

extern ACLStruct *acl_array;
extern size_t acl_size, acl_capacity;

extern MACStruct *mac_array;
extern size_t mac_size, mac_capacity;

#endif // GLOBAL_VAR_H