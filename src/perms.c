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

/**
 * Functions specific for Discretionary Access Control (DAC):
 */

bool get_dac(){ 
    char path[MAX_FILEPATH_SIZE];
    char options[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    if(get_filepath(path) && get_option(options)){
        printf("MSG: Press q to exit current view.\n");
        snprintf(command, sizeof(command), "ls \"%s\" -- \"%s\" | less", options, path);
        system(command);
        return true;
    }else{
        printf("ERR: DAC could not be retrieved.\n");
        return false;
    }
}

bool set_dac(){
    char path[MAX_FILEPATH_SIZE];
    char permission[6];                 // Needed for /n and /0?
    char command[MAX_LINE_LENGTH];
    char user[MAX_NAME_LENGTH];
    char group[MAX_NAME_LENGTH];
    char options[MAX_LINE_LENGTH];

    if(get_filepath(path) && path_exists(path) && get_option(options)){
        get_user_input("MSG: Please enter the permission (e.g. 0777):", permission, 6);
        get_user_input("MSG: Please enter the target user:", user, MAX_NAME_LENGTH);
        get_user_input("MSG: Please enter the target group:", group, MAX_NAME_LENGTH);
        if(check_permission(permission) && check_user(user) && check_group(group)){
            printf("MSG: Setting DAC...\n");
            snprintf(command, sizeof(command), "chmod %s %s %s", options, permission, path);
            add_config_command(command);
            snprintf(command, sizeof(command), "chown %s %s:%s %s\n", options, user, group, path);
            add_config_command(command);
            return true;
        }else{
            printf("ERR: DAC could not be set.\n");
            return false;
        }
    }else{
        printf("ERR: Invalid/non-existent path.\n");
        return false;
    }
}

/**
 * Functions specific to Access Control Lists:
 */

 const char *dicc_no_acl_fs[] = {         // Filesystems that do not support ACLs.
    "tmpfs",
    "vfat",
    "exfat",
    "iso9660",
    "squashfs",
    "msdosfs",
    "procfs",
    "sysfs",
    NULL
};


bool get_acl(){
    char path[MAX_FILEPATH_SIZE];
    char options[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    if(get_filepath(path) && get_option(options)){
        printf("MSG: Press q to exit current view.\n");
        snprintf(command, sizeof(command), "getfacl \"%s\" \"%s\" | less", options, path);
        system(command);
        return true;
    }else{
        printf("ERR: ACL could not be retrieved.\n");
        return false;
    }
}

//bool set_acl() is OS-specific.

bool acl_incompatible_fs(char *fp){
    bool result = false;
    for(int i=0; i < get_diccionary_size(dicc_no_acl_fs) && !result; i++){
        result = find_strings_in_line(fp,dicc_no_acl_fs[i],"/etc/fstab");
    }
    return result;
}

/**
 * TODO: Implement function(s) that check if there are conflicts between ACL
 * and the current DAC policy. This is important because DAC can override the
 * current ACL policy, which can lead to security issues.
 * Linux does not warn about this, but it is a good practice to check for conflicts.
 * This is especially important for the root user, as it can override any ACL policy.
 */