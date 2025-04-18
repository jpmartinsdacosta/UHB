#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <time.h>

#include "menu.h"
#include "file.h"
#include "config.h"
#include "utils.h"
#include "input.h"
#include "os_interface.h"
#include "global_var.h"

typedef struct {
    char fp[MAX_FILEPATH_SIZE];         // Filepath
    char user[MAX_NAME_LENGTH];
    char group[MAX_NAME_LENGTH];
    char dac[6];
    char fs[30];                        // Filesystem
    time_t timestamp;
} UhbStruct;

// ACLStruct N:1 UhbStruct

typedef struct {
    UhbStruct parent;
} ACLStruct;

UhbStruct *perms_list = NULL;

/**
 * Functions specific for UhbStruct
 */

UhbStruct* allocate_mem(size_t capacity){
    return (UhbStruct *)malloc(capacity * sizeof(UhbStruct));
}

UhbStruct* reallocate_mem(UhbStruct *array, size_t new_capacity) {
    return (UhbStruct *)realloc(array, new_capacity * sizeof(UhbStruct));
}

bool add_struct(UhbStruct **array, size_t *size, size_t *capacity){
    if(*size == *capacity){
        *capacity *= 2;
        *array = reallocate_mem(*array, *capacity);
        if (*array == NULL) {
            perror("Failed to reallocate memory");
            return false;
        }
    }
    (*size)++;
    return true;
}

bool add_dac_struct(UhbStruct *perm, char *fp, char *user, char *group, char *dac){
    if (perm == NULL || fp == NULL || user == NULL || group == NULL || dac == NULL) {
        fprintf(stderr, "Null pointer passed to add_dac_struct.\n");
        return false;
    }
    strncpy(perm->fp, fp, sizeof(perm->fp) - 1);
    perm->fp[sizeof(perm->fp) - 1] = '\0';
    strncpy(perm->user, user, sizeof(perm->user) - 1);
    perm->user[sizeof(perm->user) - 1] = '\0';
	strncpy(perm->group, group, sizeof(perm->group) - 1);
    perm->group[sizeof(perm->group) - 1] = '\0';
	strncpy(perm->dac, dac, sizeof(perm->dac) - 1);
    perm->dac[sizeof(perm->dac) - 1] = '\0';
    return true;
}

bool add_time_struct(UhbStruct *perm){
    time_t current = time(NULL);
    if (perm == NULL) {
        fprintf(stderr, "Null pointer passed to get_current_time.\n");
        return false;
    }
    if (current == (time_t)-1) {
        fprintf(stderr, "Timestamp is not set.\n");
        return false;
    }
    perm->timestamp = current;
    return true;
}

bool get_time(UhbStruct *perm) {
    if (perm == NULL) {
        fprintf(stderr, "Null pointer passed to get_current_time.\n");
        return false;
    }

    // Check if the timestamp is valid
    if (perm->timestamp == (time_t)-1) {
        fprintf(stderr, "Timestamp is not set.\n");
        return false;
    }

    struct tm *timeInfo = localtime(&perm->timestamp);
    if (timeInfo == NULL) {
        fprintf(stderr, "Failed to convert timestamp to local time.\n");
        return false;
    }

    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeInfo);
    printf("Readable Timestamp: %s\n", timeStr);
    return true;
}