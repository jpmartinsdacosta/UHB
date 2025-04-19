#include <time.h>

#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

/**
 * @file global_vars.h
 * @brief Header file responsible for declaring extern global constants used throughout UHB.
 */

#define MAX_FILEPATH_SIZE 4096  // 4096 characters max. in Linux
#define MAX_OPTIONS_LENGTH 100  // 100, for the large ACL options list.
#define MAX_LINE_LENGTH 2048    // 2048, to be used for commands too.
#define MAX_NAME_LENGTH 32      // 32, standard.

struct DACStruct {
    char fp[MAX_FILEPATH_SIZE];
    char user[MAX_NAME_LENGTH];
    char group[MAX_NAME_LENGTH];
    char dac[6];
    time_t timestamp;
};

struct ACLStruct {
    char fp[MAX_FILEPATH_SIZE];
    char fs[30];
    char acl[MAX_LINE_LENGTH];
    time_t timestamp;
};

#endif // GLOBAL_VARS_H