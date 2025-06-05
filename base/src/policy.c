#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "global_var.h"
#include "file.h"

/**
 * Flag management functions
 */

void init_fc(FlagCollection *fc, int numFlags) {
    fc->flags = (Flag *)malloc(numFlags * sizeof(Flag));
    fc->count = numFlags;
    for (int i = 0; i < numFlags; i++) {
        fc->flags[i].used = false;
    }
    printf("DBG: Flag collection initialized.\n");
}

void free_fc(FlagCollection *fc) {
    free(fc->flags);
    fc->count = 0;
    printf("DBG: Flag collection freed.\n");
}

void set_flags(FlagCollection *fc, const char *flagArray) {
    for (int i = 0; i < fc->count; i++) {
        fc->flags[i].flag = flagArray[i];
    }
}

void reset_flag_used(FlagCollection *fc) {
    for (int i = 0; i < fc->count; i++) {
        fc->flags[i].used = false;
    }
}

void print_flags(FlagCollection *fc){
    printf("MSG: Available flag options: ");
    for(int i = 0; i < fc->count; i++){
        printf("%c ",fc->flags[i].flag);
    }
    printf("\n");
}

void init_flag(FlagCollection *fc, int numFlags, const char *flagArray){
    init_fc(fc,numFlags);
    set_flags(fc,flagArray);
    print_flags(fc);
}

bool find_flag(char flag, FlagCollection *fc) {
    for (int i = 0; i < fc->count; i++) {
        if (flag == fc->flags[i].flag && !fc->flags[i].used) {
            fc->flags[i].used = true;
            return true;
        }
        if (flag == fc->flags[i].flag && fc->flags[i].used) {
            fprintf(stderr,"ERR: find_flag(): Duplicate flag found.\n");
            return false;
        }
    }
    fprintf(stderr,"ERR: find_flag(): Incompatible flag found.\n");
    return false;
}

bool check_flags(char *command, FlagCollection *fc) {
    bool correct = true;
    size_t i = 0;
    if(command[i] == '\n' || command[i] == '\0'){   // No flags were given
        return correct;
    }
    if (command[i] != '-') {                        // Flags were given, must check validity
        correct = false;
    } else {
        i++;
    }
    while (command[i] != '\0' && correct) {
        correct = find_flag(command[i], fc);
        i++;
    }
    reset_flag_used(fc);    // Once checked, make sure to reset all flags ...
    free_fc(fc);            // and free the FlagCollection
    return correct;
}

// Memory allocation helpers
void* alloc_struct(size_t capacity, size_t element_size) {
    return malloc(capacity * element_size);
}

void* realloc_struct(void *structure, size_t new_capacity, size_t element_size) {
    if (new_capacity == 0) {
        free(structure);
        return NULL;
    }
    return realloc(structure, new_capacity * element_size);
}

/**
 * DACStruct functions
 */

DACStruct *dac_array = NULL;
size_t dac_size = 0;
size_t dac_capacity = 0;

bool init_dac_array() {
    dac_capacity = 1;
    dac_array = alloc_struct(dac_capacity, sizeof(DACStruct));
    if (!dac_array) return false;
    dac_size = 0;
    return true;
}

void clear_dac_array() {
    free(dac_array);
    dac_array = NULL;
    dac_size = 0;
    dac_capacity = 0;
}

void get_dac_data(size_t i) {
    if (i >= dac_size) return;
    DACStruct *d = &dac_array[i];
    printf("Filepath: %s\nUser: %s\nGroup: %s\nDAC: %s\nRecursive: %s\nTimestamp: %s",
           d->fp, d->user, d->group, d->dac, d->recursive ? "true" : "false", ctime(&d->timestamp));
}

bool is_dac_contained(const char *fp) {
    for (size_t i = 0; i < dac_size; ++i) {
        if (dac_array[i].recursive && is_contained(dac_array[i].fp, fp)) return true;
    }
    return false;
}

bool add_dac_element(const char *fp, const char *user, const char *group, const char *dac, bool recursive) {
    if (!fp || !user || !group || !dac) return false;

    if (dac_size == dac_capacity) {
        dac_capacity *= 2;
        dac_array = realloc_struct(dac_array, dac_capacity, sizeof(DACStruct));
        if (!dac_array) return false;
    }

    DACStruct *d = &dac_array[dac_size++];
    strncpy(d->fp, fp, MAX_FILEPATH_SIZE - 1); d->fp[MAX_FILEPATH_SIZE - 1] = '\0';
    strncpy(d->user, user, MAX_NAME_LENGTH - 1);
    strncpy(d->group, group, MAX_NAME_LENGTH - 1);
    strncpy(d->dac, dac, 5); d->dac[5] = '\0';
    d->recursive = recursive || is_dac_contained(fp);
    d->timestamp = time(NULL);
    get_dac_data(dac_size - 1);
    return true;
}

bool rem_dac_element() {
    if (dac_size == 0) return true;
    --dac_size;
    if (dac_size == 0) {
        free(dac_array);
        dac_array = NULL;
        dac_capacity = 0;
    } else {
        dac_array = realloc_struct(dac_array, dac_size, sizeof(DACStruct));
    }
    return true;
}

int find_dac_index_by_filepath(const char *fp) {
    for (size_t i = 0; i < dac_size; ++i) {
        if (strcmp(dac_array[i].fp, fp) == 0) return (int)i;
    }
    return -1;
}

/**
 * ACLStruct functions
 */

ACLStruct *acl_array = NULL;
size_t acl_size = 0;
size_t acl_capacity = 0;

bool init_acl_array() {
    acl_capacity = 1;
    acl_array = alloc_struct(acl_capacity, sizeof(ACLStruct));
    if (!acl_array) return false;
    acl_size = 0;
    return true;
}

void clear_acl_array() {
    free(acl_array);
    acl_array = NULL;
    acl_size = 0;
    acl_capacity = 0;
}

void get_acl_data(size_t i) {
    if (i >= acl_size) return;
    ACLStruct *a = &acl_array[i];
    printf("ACL: %s\nFilepath: %s\nRecursive: %s\nTimestamp: %s",
           a->acl, a->fp, a->recursive ? "true" : "false", ctime(&a->timestamp));
}


bool add_acl_element(const char *fp, const char *acl) {
    if (!fp || !acl) return false;

    if (acl_size == acl_capacity) {
        acl_capacity = (acl_capacity == 0) ? 1 : acl_capacity * 2;
        acl_array = realloc_struct(acl_array, acl_capacity, sizeof(ACLStruct));
        if (!acl_array) return false;
    }

    ACLStruct *a = &acl_array[acl_size++];
    strncpy(a->fp, fp, MAX_FILEPATH_SIZE - 1); a->fp[MAX_FILEPATH_SIZE - 1] = '\0';
    strncpy(a->acl, acl, MAX_LINE_LENGTH - 1); a->acl[MAX_LINE_LENGTH - 1] = '\0';
    a->recursive = false;
    a->timestamp = time(NULL);  // Set timestamp

    get_acl_data(acl_size - 1);
    return true;
}


bool rem_acl_element() {
    if (acl_size == 0) return true;

    --acl_size;

    ACLStruct *new_array = realloc_struct(acl_array, acl_size, sizeof(ACLStruct));
    if (!new_array && acl_size != 0) {
        perror("Failed to reallocate memory for ACLStruct");
        return false;
    }

    acl_array = new_array;
    return true;
}

int find_acl_index_by_filepath(const char *fp) {
    for (size_t i = 0; i < acl_size; ++i) {
        if (strcmp(acl_array[i].fp, fp) == 0) return (int)i;
    }
    return -1;
}

/**
 * MACStruct functions
 */

MACStruct *mac_array = NULL;
size_t mac_size = 0;
size_t mac_capacity = 0;

bool init_mac_array() {
    mac_capacity = 1;
    mac_array = alloc_struct(mac_capacity, sizeof(MACStruct));
    if (!mac_array) return false;
    mac_size = 0;
    return true;
}

void clear_mac_array() {
    free(mac_array);
    mac_array = NULL;
    mac_size = 0;
    mac_capacity = 0;
}

void get_mac_data(size_t i) {
    if (i >= mac_size) return;
    MACStruct *m = &mac_array[i];
    printf("MAC: %s\nFilepath: %s\nRecursive: %s\nTimestamp: %s",
           m->mac, m->fp, m->recursive ? "true" : "false", ctime(&m->timestamp));
}


bool add_mac_element(const char *fp, const char *mac) {
    if (!fp || !mac) return false;

    if (mac_size == mac_capacity) {
        mac_capacity = (mac_capacity == 0) ? 1 : mac_capacity * 2;
        mac_array = realloc_struct(mac_array, mac_capacity, sizeof(MACStruct));
        if (!mac_array) return false;
    }

    MACStruct *m = &mac_array[mac_size++];
    strncpy(m->fp, fp, MAX_FILEPATH_SIZE - 1); m->fp[MAX_FILEPATH_SIZE - 1] = '\0';
    strncpy(m->mac, mac, MAX_LINE_LENGTH - 1); m->mac[MAX_LINE_LENGTH - 1] = '\0';
    m->recursive = false;
    m->timestamp = time(NULL);  // Set timestamp

    get_mac_data(mac_size - 1);
    return true;
}


bool rem_mac_element() {
    if (mac_size == 0) return true;

    --mac_size;

    MACStruct *new_array = realloc_struct(mac_array, mac_size, sizeof(MACStruct));
    if (!new_array && mac_size != 0) {
        perror("Failed to reallocate memory for MACStruct");
        return false;
    }

    mac_array = new_array;
    return true;
}

int find_mac_index_by_filepath(const char *fp) {
    for (size_t i = 0; i < acl_size; ++i) {
        if (strcmp(mac_array[i].fp, fp) == 0) return (int)i;
    }
    return -1;
}

void init_all_arrays(){
    init_dac_array();
    init_acl_array();
    init_mac_array();
}

void clear_all_arrays(){
    clear_dac_array();
    clear_acl_array();
    clear_mac_array();
}