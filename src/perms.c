#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "perms.h"
#include "global_var.h"

struct UHBStruct *uhb_array = NULL;
size_t uhb_size = 0;
size_t uhb_capacity = 0;

/**
 * Memory allocation functions
 */

void* alloc_struct(size_t capacity, size_t element_size) {
    return malloc(capacity * element_size);
}

void* realloc_struct(void *structure, size_t new_capacity, size_t element_size) {
    return realloc(structure, new_capacity * element_size);
}

/**
 * UHBStruct functions
 */

bool init_uhb(struct UHBStruct *instance, const char *filepath) {
    if (instance == NULL || filepath == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to init_uhb().\n");
        return false;
    }
    strncpy(instance->fp, filepath, sizeof(instance->fp) - 1);
    instance->fp[sizeof(instance->fp) - 1] = '\0';
    instance->dac_array = NULL;
    instance->dac_size = 0;
    instance->dac_capacity = 0;
    instance->acl_array = NULL;
    instance->acl_size = 0;
    instance->acl_capacity = 0;
    instance->mac_array = NULL;
    instance->mac_size = 0;
    instance->mac_capacity = 0;
    instance->timestamp = time(NULL);
    return true;
}

void clear_uhb(struct UHBStruct *instance) {
    if (instance == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to clear_uhb().\n");
        return;
    }
    free(instance->dac_array);
    instance->dac_array = NULL;
    instance->dac_size = 0;
    instance->dac_capacity = 0;
    free(instance->acl_array);
    instance->acl_array = NULL;
    instance->acl_size = 0;
    instance->acl_capacity = 0;
    free(instance->mac_array);
    instance->mac_array = NULL;
    instance->mac_size = 0;
    instance->mac_capacity = 0;
}

bool init_uhb_array() {
    uhb_capacity = 1;
    uhb_array = (struct UHBStruct *)alloc_struct(uhb_capacity, sizeof(struct UHBStruct));
    if (uhb_array == NULL) {
        perror("Failed to allocate memory for UHBStruct array.\n");
        return false;
    }
    uhb_size = 0;
    printf("DBG: Successfully initialized array of permissions!\n");
    return true;
}

void clear_uhb_array() {
    for (size_t i = 0; i < uhb_size; i++) {
        clear_uhb(&uhb_array[i]);
    }
    free(uhb_array);
    uhb_array = NULL;
    uhb_size = 0;
    uhb_capacity = 0;
    printf("DBG: Successfully cleared array of permissions!\n");
}

bool add_uhb_element(const char *filepath) {
    if (filepath == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to add_uhb_element().\n");
        return false;
    }
    if (uhb_size == uhb_capacity) {
        uhb_capacity *= 2;
        uhb_array = (struct UHBStruct *)realloc_struct(uhb_array, uhb_capacity, sizeof(struct UHBStruct));
        if (uhb_array == NULL) {
            perror("Failed to reallocate memory for UHBStruct array.\n");
            return false;
        }
    }
    if (!init_uhb(&uhb_array[uhb_size], filepath)) {
        return false;
    }
    uhb_size++;
    return true;
}

/*
void get_uhb_data(int index) {
    if (index < 0 || index >= uhb_size) {
        fprintf(stderr, "ERR: Index %d is out of bounds.\n", index);
        return;
    }
    printf("Filepath: %s\n", uhb_array[index].fp);
    printf("Timestamp: %s", ctime(&uhb_array[index].timestamp));
    printf("DACStruct Array:\n");
    for (size_t i = 0; i < uhb_array[index].dac_size; i++) {
        get_dac_data(index, i);
        printf("\n");
    }
}
*/

bool rem_uhb_element() {
    if (uhb_size == 0) {
        return true;
    }
    uhb_size--;
    clear_uhb(&uhb_array[uhb_size]);
    uhb_array = (struct UHBStruct *)realloc_struct(uhb_array, uhb_size, sizeof(struct UHBStruct));
    if (uhb_array == NULL && uhb_size != 0) {
        perror("Failed to reallocate memory for UHBStruct array");
        return false;
    }
    return true;
}

bool uhb_filepath_exists(const char *filepath){
    bool found = false;
    if (uhb_size == 0) {
        return false;
    }else{
        for (size_t i = 0; i < uhb_size; i++) {
            if(strcmp(filepath,uhb_array[i].fp) == 0){
                found = true;
            }
        }
    }
    return found;
}

/**
 * DACStruct functions
 */

bool add_dac_element(size_t uhb_index, char *user, char *group, char *dac) {
    if (uhb_index >= uhb_size || user == NULL || group == NULL || dac == NULL) {
        fprintf(stderr, "ERR: Invalid parameters passed to add_dac_element().\n");
        return false;
    }
    struct UHBStruct *uhb = &uhb_array[uhb_index];
    if (uhb->dac_size == uhb->dac_capacity) {
        uhb->dac_capacity++;
        uhb->dac_array = (struct DACStruct *)realloc_struct(uhb->dac_array, uhb->dac_capacity, sizeof(struct DACStruct));
        if (uhb->dac_array == NULL) {
            perror("Failed to add memory for DACStruct.\n");
            return false;
        }
    }
    struct DACStruct *element = &uhb->dac_array[uhb->dac_size];
    strncpy(element->user, user, sizeof(element->user) - 1);
    element->user[sizeof(element->user) - 1] = '\0';
    strncpy(element->group, group, sizeof(element->group) - 1);
    element->group[sizeof(element->group) - 1] = '\0';
    strncpy(element->dac, dac, sizeof(element->dac) - 1);
    element->dac[sizeof(element->dac) - 1] = '\0';
    uhb->dac_size++;
    return true;
}

void get_dac_data(size_t uhb_index, size_t dac_index) {
    if (uhb_index >= uhb_size || dac_index >= uhb_array[uhb_index].dac_size) {
        fprintf(stderr, "ERR: Index out of bounds.\n");
        return;
    }
    struct UHBStruct *uhb = &uhb_array[uhb_index];
    printf("User: %s\n", uhb->dac_array[dac_index].user);
    printf("Group: %s\n", uhb->dac_array[dac_index].group);
    printf("DAC Permissions: %s\n", uhb->dac_array[dac_index].dac);
}

bool rem_dac_element(size_t uhb_index) {
    if (uhb_index >= uhb_size) {
        fprintf(stderr, "ERR: Invalid UHBStruct index.\n");
        return false;
    }
    struct UHBStruct *uhb = &uhb_array[uhb_index];
    if (uhb->dac_size == 0) {
        return true;
    }
    uhb->dac_size--;
    uhb->dac_array = (struct DACStruct *)realloc_struct(uhb->dac_array, uhb->dac_size, sizeof(struct DACStruct));
    if (uhb->dac_array == NULL && uhb->dac_size != 0) {
        perror("Failed to reallocate memory for DACStruct");
        return false;
    }
    return true;
}

/**
 * ACLStruct functions TBD
 */

/**
 * MACStruct functions TBD
 */

/**
 * Policy-checking functions
 */
