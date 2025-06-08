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
    char flag[MAX_LINE_LENGTH];     ///< Operation type (e.g., read, write, modify).
    char entry[MAX_LINE_LENGTH];    ///< Target entity (user/group/role).
    char fp[MAX_FILEPATH_SIZE];     ///< Absolute or relative file path.
    bool recursive;                 ///< Indicates if the policy applies recursively to subdirectories.
    time_t timestamp;               ///< Time when the entry was created or modified.
} ACLStruct;

typedef struct {
    char fp[MAX_FILEPATH_SIZE];     ///< File or object path being secured.
    char input[MAX_LINE_LENGTH];    ///< Original input command or rule (optional metadata).
    char subject[MAX_NAME_LENGTH];  ///< Subject (e.g., process, user role).
    char uid[MAX_NAME_LENGTH];      ///< User ID related to the subject.
    char gid[MAX_NAME_LENGTH];      ///< Group ID related to the subject.
    char object[MAX_NAME_LENGTH];   ///< Target object (e.g., file, resource).
    char type[MAX_LINE_LENGTH];     ///< Type of access or label (e.g., SE label, category).
    char mode[MAX_LINE_LENGTH];     ///< Mode of enforcement (e.g., read-only, enforce, permissive).
    bool recursive;                 ///< Applies recursively if true.
    time_t timestamp;               ///< Entry creation/modification timestamp.
} MACStruct;

typedef struct {
    char fp[MAX_FILEPATH_SIZE];     ///< Path to the file or directory.
    char user[MAX_NAME_LENGTH];     ///< Owner user name.
    char group[MAX_NAME_LENGTH];    ///< Owner group name.
    char dac[6];                    ///< Permission string (e.g., "0755" in octal).
    bool recursive;                 ///< If true, applies to all subdirectories and files.
    time_t timestamp;               ///< Time of entry addition or update.
} DACStruct;

// Global policy arrays
extern DACStruct *dac_array;
extern size_t dac_size, dac_capacity;

extern ACLStruct *acl_array;
extern size_t acl_size, acl_capacity;

extern MACStruct *mac_array;
extern size_t mac_size, mac_capacity;

#endif // GLOBAL_VAR_H