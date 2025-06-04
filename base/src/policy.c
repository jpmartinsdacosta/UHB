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

/**
 * Generic struct memory allocation functions
 */
size_t dac_size = 0;
size_t dac_capacity = 0;
struct DACStruct *dac_array = NULL;

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

bool init_dac_array() {
    dac_capacity = 1;
    dac_array = alloc_struct(dac_capacity, sizeof(struct DACStruct));
    if (!dac_array) {
        perror("Failed to allocate memory for DACStruct array.");
        return false;
    }
    dac_size = 0;
    printf("DBG: DACStruct successfully initialized.\n");
    return true;
}

void clear_dac_array() {
    for (size_t i = 0; i < dac_size; ++i) {
        free(dac_array[i].acl_array);
        free(dac_array[i].mac_array);
    }
    free(dac_array);
    dac_array = NULL;
    dac_size = 0;
    dac_capacity = 0;
    printf("DBG: DACStruct successfully cleared.\n");
}

void get_dac_data(size_t dac_index) {
    if (dac_index >= dac_size) {
        fprintf(stderr, "ERR: Index out of bounds.\n");
        return;
    }

    struct DACStruct *dac = &dac_array[dac_index];
    printf("Filepath: %s\n", dac->fp);
    printf("User: %s\n", dac->user);
    printf("Group: %s\n", dac->group);
    printf("DAC Permissions: %s\n", dac->dac);
    printf("Recursive?: %s\n", dac->recursive ? "true" : "false");
    printf("Timestamp: %s", ctime(&dac->timestamp));  // no need for \n
}

bool is_dac_contained(const char *filepath) {
    if (!filepath) return false;

    for (size_t i = 0; i < dac_size; ++i) {
        printf("DBG: Is %s contained inside %s?\n", filepath, dac_array[i].fp);
        if (dac_array[i].recursive && is_contained(dac_array[i].fp, filepath)) {
            printf("DBG: Yes, it is contained!\n");
            return true;
        }
    }
    return false;
}

bool add_dac_element(const char *filepath, const char *user, const char *group, const char *dac, bool recursive) {
    if (!filepath || !user || !group || !dac) {
        fprintf(stderr, "ERR: Null pointer passed to add_dac_element().\n");
        return false;
    }

    if (dac_size == dac_capacity) {
        size_t new_capacity = dac_capacity * 2;
        struct DACStruct *new_array = realloc_struct(dac_array, new_capacity, sizeof(struct DACStruct));
        if (!new_array) {
            perror("Failed to reallocate memory for DACStruct array.");
            return false;
        }
        dac_array = new_array;
        dac_capacity = new_capacity;
    }

    struct DACStruct *element = &dac_array[dac_size];
    strncpy(element->fp, filepath, sizeof(element->fp) - 1); element->fp[sizeof(element->fp) - 1] = '\0';
    strncpy(element->user, user, sizeof(element->user) - 1); element->user[sizeof(element->user) - 1] = '\0';
    strncpy(element->group, group, sizeof(element->group) - 1); element->group[sizeof(element->group) - 1] = '\0';
    strncpy(element->dac, dac, sizeof(element->dac) - 1); element->dac[sizeof(element->dac) - 1] = '\0';

    element->recursive = recursive || is_dac_contained(filepath);
    if (element->recursive) {
        printf("Contained, adding recursive option...\n");
    }

    element->acl_array = NULL;
    element->acl_size = 0;
    element->acl_capacity = 0;
    element->mac_array = NULL;
    element->mac_size = 0;
    element->mac_capacity = 0;
    element->timestamp = time(NULL);

    dac_size++;
    get_dac_data(dac_size - 1);
    return true;
}

bool rem_dac_element() {
    if (dac_size == 0) return true;

    --dac_size;
    free(dac_array[dac_size].acl_array);
    free(dac_array[dac_size].mac_array);

    if (dac_size == 0) {
        free(dac_array);
        dac_array = NULL;
        dac_capacity = 0;
    } else {
        struct DACStruct *new_array = realloc_struct(dac_array, dac_size, sizeof(struct DACStruct));
        if (!new_array && dac_size != 0) {
            perror("Failed to reallocate memory for DACStruct array");
            return false;
        }
        dac_array = new_array;
    }

    return true;
}

int find_dac_index_by_filepath(const char *filepath) {
    if (!filepath) {
        fprintf(stderr, "ERR: Null pointer passed to find_dac_index_by_filepath().\n");
        return -1;
    }
    for (size_t i = 0; i < dac_size; ++i) {
        if (strcmp(dac_array[i].fp, filepath) == 0) {
            return (int)i;
        }
    }
    return -1;
}

/**
 * ACLStruct functions
 */

void get_acl_data(size_t dac_index, size_t acl_index) {
    if (dac_index >= dac_size || acl_index >= dac_array[dac_index].acl_size) {
        fprintf(stderr, "ERR: Index out of bounds.\n");
        return;
    }

    struct ACLStruct *acl = &dac_array[dac_index].acl_array[acl_index];
    printf("ACL: %s\n", acl->acl);
    printf("Recursive?: %s\n", acl->recursive ? "true" : "false");
}

bool add_acl_element(const char *filepath, const char *acl) {
    if (filepath == NULL || acl == NULL) {
        fprintf(stderr, "ERR: Invalid parameters passed to add_acl_element().\n");
        return false;
    }

    int index = find_dac_index_by_filepath(filepath);
    if (index == -1) {
        if (!add_dac_element(filepath, "default_user", "default_group", "rwx", false)) {
            return false;
        }
        index = (int)(dac_size - 1);
    }

    struct DACStruct *dac = &dac_array[index];

    if (dac->acl_size == dac->acl_capacity) {
        dac->acl_capacity++;
        dac->acl_array = (struct ACLStruct *)realloc_struct(dac->acl_array, dac->acl_capacity, sizeof(struct ACLStruct));
        if (dac->acl_array == NULL) {
            perror("Failed to reallocate memory for ACLStruct.\n");
            return false;
        }
    }

    struct ACLStruct *element = &dac->acl_array[dac->acl_size];
    strncpy(element->acl, acl, sizeof(element->acl) - 1);
    element->acl[sizeof(element->acl) - 1] = '\0';
    element->recursive = false;
    dac->acl_size++;
    get_acl_data(index, dac->acl_size - 1);
    return true;
}


bool rem_acl_element(size_t dac_index, size_t acl_index) {
    if (dac_index >= dac_size || acl_index >= dac_array[dac_index].acl_size) {
        fprintf(stderr, "ERR: Index out of bounds.\n");
        return false;
    }

    struct DACStruct *dac = &dac_array[dac_index];
    if (acl_index < dac->acl_size - 1) {
        memmove(&dac->acl_array[acl_index], &dac->acl_array[acl_index + 1],
                (dac->acl_size - acl_index - 1) * sizeof(struct ACLStruct));
    }
    dac->acl_size--;

    struct ACLStruct *new_array = realloc_struct(dac->acl_array, dac->acl_size, sizeof(struct ACLStruct));
    if (!new_array && dac->acl_size != 0) {
        perror("Failed to reallocate memory for ACLStruct");
        return false;
    }
    dac->acl_array = new_array;
    return true;
}

/**
 * MACStruct functions
 */

void get_mac_data(size_t dac_index, size_t mac_index) {
    if (dac_index >= dac_size || mac_index >= dac_array[dac_index].mac_size) {
        fprintf(stderr, "ERR: Index out of bounds.\n");
        return;
    }

    struct MACStruct *mac = &dac_array[dac_index].mac_array[mac_index];
    printf("MAC: %s\n", mac->mac);
    printf("Recursive?: %s\n", mac->recursive ? "true" : "false");
}

bool add_mac_element(const char *filepath, const char *mac) {
    if (filepath == NULL || mac == NULL) {
        fprintf(stderr, "ERR: Invalid parameters passed to add_mac_element().\n");
        return false;
    }

    int index = find_dac_index_by_filepath(filepath);
    if (index == -1) {
        if (!add_dac_element(filepath, "default_user", "default_group", "rwx", false)) {
            return false;
        }
        index = (int)(dac_size - 1);
    }

    struct DACStruct *dac = &dac_array[index];

    if (dac->mac_size == dac->mac_capacity) {
        dac->mac_capacity++;
        dac->mac_array = (struct MACStruct *)realloc_struct(dac->mac_array, dac->mac_capacity, sizeof(struct MACStruct));
        if (dac->mac_array == NULL) {
            perror("Failed to reallocate memory for MACStruct.\n");
            return false;
        }
    }

    struct MACStruct *element = &dac->mac_array[dac->mac_size];
    strncpy(element->mac, mac, sizeof(element->mac) - 1);
    element->mac[sizeof(element->mac) - 1] = '\0';
    element->recursive = false;
    dac->mac_size++;
    get_mac_data(index, dac->mac_size - 1);
    return true;
}


bool rem_mac_element(size_t dac_index, size_t mac_index) {
    if (dac_index >= dac_size || mac_index >= dac_array[dac_index].mac_size) {
        fprintf(stderr, "ERR: Index out of bounds.\n");
        return false;
    }

    struct DACStruct *dac = &dac_array[dac_index];
    if (mac_index < dac->mac_size - 1) {
        memmove(&dac->mac_array[mac_index], &dac->mac_array[mac_index + 1],
                (dac->mac_size - mac_index - 1) * sizeof(struct MACStruct));
    }
    dac->mac_size--;

    struct MACStruct *new_array = realloc_struct(dac->mac_array, dac->mac_size, sizeof(struct MACStruct));
    if (!new_array && dac->mac_size != 0) {
        perror("Failed to reallocate memory for MACStruct");
        return false;
    }
    dac->mac_array = new_array;
    return true;
}