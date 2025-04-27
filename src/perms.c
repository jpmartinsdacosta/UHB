#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "perms.h"
#include "global_var.h"


static size_t uhb_size = 0;
static size_t uhb_capacity = 0;

/**
 * Functions common to all
 */

void* alloc_struct(size_t capacity, size_t size) {
    return malloc(capacity * size);
}

void* realloc_struct(void *structure, size_t new_capacity, size_t size) {
    return realloc(structure, new_capacity * size);
}

/**
 * UHBStruct functions
 */

bool init_uhb_array() {
    uhb_capacity = 1;
    uhb = (struct UHBStruct *)alloc_struct(uhb_capacity, sizeof(struct UHBStruct));
    if (uhb == NULL) {
        perror("Failed to allocate memory for UHBStruct array.\n");
        return false;
    }
    return true;
}

bool init_uhb_element_array(const char *fp) {
    if (uhb == NULL || fp == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to init_uhb_element_array().\n");
        return false;
    }
    strncpy(uhb->fp, fp, sizeof(uhb->fp) - 1);
    uhb->fp[sizeof(uhb->fp) - 1] = '\0';
    uhb->dac_array = NULL;
    uhb->dac_size = 0;
    uhb->dac_capacity = 0;
    uhb->acl_array = NULL;
    uhb->acl_size = 0;
    uhb->acl_capacity = 0;
    uhb->mac_array = NULL;
    uhb->mac_size = 0;
    uhb->mac_capacity = 0;
    uhb->timestamp = time(NULL);
    return true;
}

void clear_uhb_element_array() {
    if (uhb == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to clear_uhb_element_array().\n");
        return;
    }
    free(uhb->dac_array);
    uhb->dac_array = NULL;
    uhb->dac_size = 0;
    uhb->dac_capacity = 0;
    free(uhb->acl_array);
    uhb->acl_array = NULL;
    uhb->acl_size = 0;
    uhb->acl_capacity = 0;
    free(uhb->mac_array);
    uhb->mac_array = NULL;
    uhb->mac_size = 0;
    uhb->mac_capacity = 0;
}

void clear_uhb_array() {
    for (size_t i = 0; i < uhb_size; i++) {
        clear_uhb_element_array(&uhb[i]);
    }
    free(uhb);
    uhb = NULL;
    uhb_size = 0;
    uhb_capacity = 0;
}

bool add_uhb_element(const char *fp) {
    if (fp == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to add_uhb_element().\n");
        return false;
    }
    if (uhb_size == uhb_capacity) {
        uhb_capacity *= 2;
        uhb = (struct UHBStruct *)realloc_struct(uhb, uhb_capacity, sizeof(struct UHBStruct));
        if (uhb == NULL) {
            perror("Failed to reallocate memory for UHBStruct array.\n");
            return false;
        }
    }
    if (!init_uhb_element_array(&uhb[uhb_size], fp)) {
        return false;
    }
    uhb_size++;
    return true;
}

void get_uhb_data(int index) {
    if (index < 0 || index >= uhb_size) {
        fprintf(stderr, "ERR: Index %d is out of bounds.\n", index);
        return;
    }
    printf("Filepath: %s\n", uhb[index].fp);
    printf("Timestamp: %s", ctime(&uhb[index].timestamp));
    printf("DACStruct Array:\n");
    for (size_t i = 0; i < uhb[index].dac_size; i++) {
        get_dac_data(&uhb[index], i);
        printf("\n");
    }
    printf("ACLStruct Array:\n");
    for (size_t i = 0; i < uhb[index].acl_size; i++) {
        get_acl_data(&uhb[index], i);
        printf("\n");
    }
    printf("MACStruct Array:\n");
    for (size_t i = 0; i < uhb[index].mac_size; i++) {
        get_mac_data(&uhb[index], i);
        printf("\n");
    }
}

bool rem_uhb_element() {
    if (uhb_size == 0) {
        return true;
    }
    uhb_size--;
    clear_uhb_element_array(&uhb[uhb_size]);
    uhb = (struct UHBStruct *)realloc_struct(uhb, uhb_size, sizeof(struct UHBStruct));
    if (uhb == NULL && uhb_size != 0) {
        perror("Failed to reallocate memory for UHBStruct array");
        return false;
    }
    return true;
}

/**
 * DACStruct functions
 */

bool add_dac_element(char *user, char *group, char *dac) {
    if (uhb == NULL || user == NULL || group == NULL || dac == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to add_dac_element().\n");
        return false;
    }
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

void get_dac_data(int index) {
    if (uhb == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to get_dac_data().\n");
        return;
    }
    if (index < 0 || index >= uhb->dac_size) {
        fprintf(stderr, "ERR: Index %d is out of bounds.\n", index);
        return;
    }
    printf("User: %s\n", uhb->dac_array[index].user);
    printf("Group: %s\n", uhb->dac_array[index].group);
    printf("DAC Permissions: %s\n", uhb->dac_array[index].dac);
}

bool rem_dac_element() {
    if (uhb == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to rem_dac_element().\n");
        return false;
    }
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
 * ACLStruct functions
 */

bool add_acl_element(char *fs, char *acl) {
    if (uhb == NULL || fs == NULL || acl == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to add_acl_element().\n");
        return false;
    }
    if (uhb->acl_size == uhb->acl_capacity) {
        uhb->acl_capacity++;
        uhb->acl_array = (struct ACLStruct *)realloc_struct(uhb->acl_array, uhb->acl_capacity, sizeof(struct ACLStruct));
        if (uhb->acl_array == NULL) {
            perror("Failed to add memory for ACLStruct.\n");
            return false;
        }
    }
    struct ACLStruct *element = &uhb->acl_array[uhb->acl_size];
    strncpy(element->fs, fs, sizeof(element->fs) - 1);
    element->fs[sizeof(element->fs) - 1] = '\0';
    strncpy(element->acl, acl, sizeof(element->acl) - 1);
    element->acl[sizeof(element->acl) - 1] = '\0';
    uhb->acl_size++;
    return true;
}

void get_acl_data(int index) {
    if (uhb == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to get_acl_data().\n");
        return;
    }
    if (index < 0 || index >= uhb->acl_size) {
        fprintf(stderr, "ERR: Index %d is out of bounds.\n", index);
        return;
    }
    printf("Filesystem: %s\n", uhb->acl_array[index].fs);
    printf("ACL: %s\n", uhb->acl_array[index].acl);
}

bool rem_acl_element() {
    if (uhb == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to rem_acl_element().\n");
        return false;
    }
    if (uhb->acl_size == 0) {
        return true;
    }
    uhb->acl_size--;
    uhb->acl_array = (struct ACLStruct *)realloc_struct(uhb->acl_array, uhb->acl_size, sizeof(struct ACLStruct));
    if (uhb->acl_array == NULL && uhb->acl_size != 0) {
        perror("Failed to reallocate memory for ACLStruct");
        return false;
    }
    return true;
}

/**
 * MACStruct functions
 */

bool add_mac_element(char *mac) {
    if (uhb == NULL || mac == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to add_mac_element().\n");
        return false;
    }
    if (uhb->mac_size == uhb->mac_capacity) {
        uhb->mac_capacity++;
        uhb->mac_array = (struct MACStruct *)realloc_struct(uhb->mac_array, uhb->mac_capacity, sizeof(struct MACStruct));
        if (uhb->mac_array == NULL) {
            perror("Failed to add memory for MACStruct.\n");
            return false;
        }
    }
    struct MACStruct *element = &uhb->mac_array[uhb->mac_size];
    strncpy(element->mac, mac, sizeof(element->mac) - 1);
    element->mac[sizeof(element->mac) - 1] = '\0';
    uhb->mac_size++;
    return true;
}

void get_mac_data(int index) {
    if (uhb == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to get_mac_data().\n");
        return;
    }
    if (index < 0 || index >= uhb->mac_size) {
        fprintf(stderr, "ERR: Index %d is out of bounds.\n", index);
        return;
    }
    printf("MAC: %s\n", uhb->mac_array[index].mac);
}

bool rem_mac_element() {
    if (uhb == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to rem_mac_element().\n");
        return false;
    }
    if (uhb->mac_size == 0) {
        return true;
    }
    uhb->mac_size--;
    uhb->mac_array = (struct MACStruct *)realloc_struct(uhb->mac_array, uhb->mac_size, sizeof(struct MACStruct));
    if (uhb->mac_array == NULL && uhb->mac_size != 0) {
        perror("Failed to reallocate memory for MACStruct");
        return false;
    }
    return true;
}

/**
 * Policy conflict checking functions
 */