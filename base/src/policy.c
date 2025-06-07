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

void print_flags(FlagCollection *fc) {
    printf("MSG: Available flag options: ");
    for (int i = 0; i < fc->count; i++) {
        printf("%c ", fc->flags[i].flag);
    }
    printf("\n");
}

void init_flag(FlagCollection *fc, int numFlags, const char *flagArray) {
    init_fc(fc, numFlags);
    set_flags(fc, flagArray);
    print_flags(fc);
}

bool find_flag(char flag, FlagCollection *fc) {
    for (int i = 0; i < fc->count; i++) {
        if (flag == fc->flags[i].flag && !fc->flags[i].used) {
            fc->flags[i].used = true;
            return true;
        }
        if (flag == fc->flags[i].flag && fc->flags[i].used) {
            fprintf(stderr, "ERR: find_flag(): Duplicate flag found.\n");
            return false;
        }
    }
    fprintf(stderr, "ERR: find_flag(): Incompatible flag found.\n");
    return false;
}

bool check_flags(char *command, FlagCollection *fc) {
    bool correct = true;
    size_t i = 0;

    if (command[i] == '\n' || command[i] == '\0') {
        return correct;
    }

    if (command[i] != '-') {
        correct = false;
    } else {
        i++;
    }

    while (command[i] != '\0' && correct) {
        correct = find_flag(command[i], fc);
        i++;
    }

    reset_flag_used(fc);
    free_fc(fc);
    return correct;
}

// Memory allocation helpers
void* alloc_struct(size_t capacity, size_t element_size) {
    return calloc(capacity, element_size);
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
    if (!dac_array || i >= dac_size) return;
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
        void *tmp = realloc_struct(dac_array, dac_capacity, sizeof(DACStruct));
        if (!tmp) return false;
        dac_array = tmp;
    }

    DACStruct *d = &dac_array[dac_size++];
    strncpy(d->fp, fp, MAX_FILEPATH_SIZE - 1); d->fp[MAX_FILEPATH_SIZE - 1] = '\0';
    strncpy(d->user, user, MAX_NAME_LENGTH - 1); d->user[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(d->group, group, MAX_NAME_LENGTH - 1); d->group[MAX_NAME_LENGTH - 1] = '\0';
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
        clear_dac_array();
    } else {
        void *tmp = realloc_struct(dac_array, dac_size, sizeof(DACStruct));
        if (tmp) dac_array = tmp;
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
    if (!acl_array || i >= acl_size) return;
    ACLStruct *a = &acl_array[i];
    printf("Flag: %s\nEntry: %s\nFilepath: %s\nRecursive: %s\nTimestamp: %s",
           a->flag, a->entry, a->fp, a->recursive ? "true" : "false", ctime(&a->timestamp));
}

bool add_acl_element(const char *flag, const char *entry, const char *fp) {
    if (!flag || !entry || !fp) return false;

    if (acl_size == acl_capacity) {
        acl_capacity = (acl_capacity == 0) ? 1 : acl_capacity * 2;
        void *tmp = realloc_struct(acl_array, acl_capacity, sizeof(ACLStruct));
        if (!tmp) return false;
        acl_array = tmp;
    }

    ACLStruct *a = &acl_array[acl_size++];
    strncpy(a->flag, flag, MAX_LINE_LENGTH - 1);
    a->flag[MAX_LINE_LENGTH - 1] = '\0';
    strncpy(a->entry, entry, MAX_LINE_LENGTH - 1);
    a->entry[MAX_LINE_LENGTH - 1] = '\0';
    strncpy(a->fp, fp, MAX_FILEPATH_SIZE - 1);
    a->fp[MAX_FILEPATH_SIZE - 1] = '\0';
    a->recursive = false;
    a->timestamp = time(NULL);

    get_acl_data(acl_size - 1);
    return true;
}

bool rem_acl_element() {
    if (acl_size == 0) return true;

    --acl_size;

    void *tmp = realloc_struct(acl_array, acl_size, sizeof(ACLStruct));
    if (!tmp && acl_size != 0) {
        perror("Failed to reallocate memory for ACLStruct");
        return false;
    }

    if (tmp) acl_array = tmp;
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
    if (!mac_array || i >= mac_size) return;
    MACStruct *m = &mac_array[i];
    printf("Filepath: %s\nInput: %s\nSubject: %s\nUID: %s\nGID: %s\nObject: %s\nType: %s\nMode: %s\nRecursive: %s\nTimestamp: %s",
           m->fp, m->input, m->subject, m->uid, m->gid, m->object, m->type, m->mode,
           m->recursive ? "true" : "false", ctime(&m->timestamp));
}
bool add_mac_element(const char *fp, const char *input, const char *subject, const char *uid, const char *gid, const char *object, const char *type, const char *mode) {
    if (!fp) return false;

    if (mac_size == mac_capacity) {
        mac_capacity = (mac_capacity == 0) ? 1 : mac_capacity * 2;
        void *tmp = realloc_struct(mac_array, mac_capacity, sizeof(MACStruct));
        if (!tmp) return false;
        mac_array = tmp;
    }

    MACStruct *m = &mac_array[mac_size++];
    strncpy(m->fp, fp, MAX_FILEPATH_SIZE - 1);
    m->fp[MAX_FILEPATH_SIZE - 1] = '\0';
    if (input) {
        strncpy(m->input, input, MAX_LINE_LENGTH - 1);
        m->input[MAX_LINE_LENGTH - 1] = '\0';
    }
    if (subject) {
        strncpy(m->subject, subject, MAX_NAME_LENGTH - 1);
        m->subject[MAX_NAME_LENGTH - 1] = '\0';
    }
    if (uid) {
        strncpy(m->uid, uid, MAX_NAME_LENGTH - 1);
        m->uid[MAX_NAME_LENGTH - 1] = '\0';
    }
    if (gid) {
        strncpy(m->gid, gid, MAX_NAME_LENGTH - 1);
        m->gid[MAX_NAME_LENGTH - 1] = '\0';
    }
    if (object) {
        strncpy(m->object, object, MAX_NAME_LENGTH - 1);
        m->object[MAX_NAME_LENGTH - 1] = '\0';
    }
    if (type) {
        strncpy(m->type, type, MAX_LINE_LENGTH - 1);
        m->type[MAX_LINE_LENGTH - 1] = '\0';
    }
    if (mode) {
        strncpy(m->mode, mode, MAX_LINE_LENGTH - 1);
        m->mode[MAX_LINE_LENGTH - 1] = '\0';
    }
    m->recursive = false;
    m->timestamp = time(NULL);

    get_mac_data(mac_size - 1);
    return true;
}

bool rem_mac_element() {
    if (mac_size == 0) return true;

    --mac_size;

    void *tmp = realloc_struct(mac_array, mac_size, sizeof(MACStruct));
    if (!tmp && mac_size != 0) {
        perror("Failed to reallocate memory for MACStruct");
        return false;
    }

    if (tmp) mac_array = tmp;
    return true;
}

int find_mac_index_by_filepath(const char *fp) {
    for (size_t i = 0; i < mac_size; ++i) {
        if (strcmp(mac_array[i].fp, fp) == 0) return (int)i;
    }
    return -1;
}

/**
 * Initialization and cleanup
 */

void init_all_arrays() {
    init_dac_array();
    init_acl_array();
    init_mac_array();
}

void clear_all_arrays() {
    clear_dac_array();
    clear_acl_array();
    clear_mac_array();
}

/**
 * Policy-checking functions
 */


