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
#define UHB_BASE_CONFIG_CURRENT "../config/current/uhb.conf"

// Backup UHB base configuration file, to be copied to current when resetting.
#define UHB_BASE_CONFIG_BACKUP  "../config/backups/uhb.conf"

// Configuration template filepaths
#define CONFIG_TEMPLATE_PATH    "../config/current/config_template.txt"

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

 struct ACLStruct {
    char fs[30];
    char acl[MAX_LINE_LENGTH];
    bool recursive;
};

struct MACStruct {
    char mac[MAX_LINE_LENGTH];
    bool recursive;
};

struct DACStruct {
    char fp[MAX_FILEPATH_SIZE];
    char user[MAX_NAME_LENGTH];
    char group[MAX_NAME_LENGTH];
    char dac[6];
    bool recursive;
    struct ACLStruct *acl_array;
    size_t acl_size, acl_capacity;
    struct MACStruct *mac_array;
    size_t mac_size, mac_capacity;
    time_t timestamp;
};

extern struct DACStruct *dac_array;

#endif // GLOBAL_VAR_H