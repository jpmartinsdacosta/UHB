#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "global_var.h"
#include "perms.h"

struct DACStruct *dac_array = NULL;
struct ACLStruct *acl_array = NULL;

static size_t dac_size = 0;
static size_t dac_capacity = 0;
static size_t acl_size = 0;
static size_t acl_capacity = 0;

/**
 * Functions common to both DAC and ACL
 */

// void* is a generic pointer type
void* alloc_struct(size_t capacity, size_t element_size){
    return malloc(capacity * element_size);
}

// void* is a generic pointer type
void* realloc_struct(void *struct_array, size_t new_capacity, size_t element_size) {
    return realloc(struct_array, new_capacity * element_size);
}

void print_timestamp(time_t timestamp) {
    struct tm *time_info = localtime(&timestamp);
    if (time_info == NULL) {
        fprintf(stderr, "Failed to convert timestamp.\n");
        return;
    }
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);
    printf("Timestamp: %s\n", time_str);
}

/**
 * Functions regarding DAC
 */

bool is_dac_empty(){
    if(dac_size == 0 && dac_capacity == 0)
        return true;
    else
        return false;
 }

bool init_dac(){
    dac_capacity = 1;
    dac_array = (struct DACStruct *)alloc_struct(dac_capacity,sizeof(struct DACStruct));
    if(dac_array == NULL){
        perror("ERR: init_dac() failed.\n");
        return false;
    }
    return true;
 }

void clear_dac(){
    free(dac_array);
    dac_array = NULL;
    dac_size = 0;
    dac_capacity = 0;
    printf("MSG: Cleared DAC struct cache.\n");
}

bool add_dac_time(int index){
    time_t current = time(NULL);
    if (dac_array == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to add_dac_time().\n");
        return false;
    }
    if (current == (time_t)-1) {
        fprintf(stderr, "ERR: Timestamp is not set.\n");
        return false;
    }
    dac_array[index].timestamp = current;
    return true;
}

bool mod_dac_element(bool add){
    if(add){                                        // Add an element to the DAC structure
        if(is_dac_empty()){                         // If empty, initialize and allocate memory.
            if(!init_dac()){
                clear_dac();                        // If initatliztion fails, CLEAR MEMORY!
                return false;
            }
        }
        if(dac_size == dac_capacity){
            dac_capacity++;
            //Update size: typecasting     reallocation   pointer   new size   size of one DACStruct
            dac_array = (struct DACStruct *)realloc_struct(dac_array, dac_capacity, sizeof(struct DACStruct));
            if(dac_array == NULL){
                perror("Failed to reallocate memory for mod_dac_element().\n");
                return false;
            }
        }
        dac_size++;
        return true;
    }else{                                          // Remove an element to the DAC structure
        if(dac_size == 0){                          // If empty, do nothing
            return true;
        }
        dac_size--;
        //Update size: typecasting     reallocation   pointer   new size   size of one DACStruct
        dac_array = (struct DACStruct *)realloc_struct(dac_array, dac_size, sizeof(struct DACStruct));
        if(dac_array == NULL && dac_size != 0){
            perror("Failed to reallocate memory for mod_dac_element().\n");
            return false;
        }
        return true;
    }
}

bool add_dac_data(char *fp, char *user, char *group, char *dac){
    if(fp == NULL || user == NULL || group == NULL || dac == NULL){
        fprintf(stderr, "ERR: Null pointer passed to add_dac_data().\n");
        return false;
    }
    if(!mod_dac_element(true)){
        return false;
    }
    struct DACStruct *element = &dac_array[dac_size - 1];
    if(element == NULL){
        fprintf(stderr, "ERR: Failed to add data to allocated memory in DACStruct.\n");
        clear_dac();
        return false;
    }
    strncpy(dac_array->fp, fp, sizeof(dac_array->fp) - 1);
    dac_array->fp[sizeof(dac_array->fp) - 1] = '\0';
    strncpy(dac_array->user, user, sizeof(dac_array->user) - 1);
    dac_array->user[sizeof(dac_array->user) - 1] = '\0';
	strncpy(dac_array->group, group, sizeof(dac_array->group) - 1);
    dac_array->group[sizeof(dac_array->group) - 1] = '\0';
	strncpy(dac_array->dac, dac, sizeof(dac_array->dac) - 1);
    dac_array->dac[sizeof(dac_array->dac) - 1] = '\0';
    add_dac_time(dac_size-1);
    get_dac_data(dac_size-1);
    return true;
}

bool rem_dac_data(){
    return mod_dac_element(false);
}

void get_dac_data(int index){
    if(dac_array == NULL){
        fprintf(stderr, "ERR: Null pointer passed to get_dac_data().\n");
    }
    if(index < 0 || index >= dac_capacity){
        fprintf(stderr, "ERR: Index out of bounds in get_dac_data().\n");
    }
        printf("Filepath: %s.\n", dac_array[index].fp);
        printf("User: %s.\n", dac_array[index].user);
        printf("Group: %s.\n", dac_array[index].group);
        printf("DAC Permissions: %s.\n", dac_array[index].dac);
        print_timestamp(dac_array[index].timestamp);
}

/**
 * Functions regarding ACL
 */

bool is_acl_empty(){
    if(acl_size == 0 && acl_capacity == 0)
        return true;
    else
        return false;
 }

bool init_acl(){
    acl_capacity = 1;
    acl_array = (struct ACLStruct *)alloc_struct(acl_capacity,sizeof(struct ACLStruct));
    if(acl_array == NULL){
        perror("ERR: init_acl() failed");
        return false;
    }
    return true;
 }

void clear_acl(){
    free(acl_array);
    acl_array = NULL;
    acl_size = 0;
    acl_capacity = 0;
}

bool add_acl_time(){
    time_t current = time(NULL);
    if (acl_array == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to add_acl_time().\n");
        return false;
    }
    if (current == (time_t)-1) {
        fprintf(stderr, "ERR: Timestamp is not set.\n");
        return false;
    }
    acl_array->timestamp = current;
    return true;
}

bool mod_acl_element(bool add){
    if(add){                                        // Add an element to the ACL structure
        if(is_acl_empty()){                         // If empty, initialize and allocate memory.
            if(!init_acl){
                clear_acl();                        // If initialization fails, CLEAR MEMORY!
                return false;
            }
        }
        if(acl_size == acl_capacity){
            acl_capacity++;
            //Update size: typecasting     reallocation   pointer   new size   size of one ACLStruct
            acl_array = (struct ACLStruct *)realloc_struct(acl_array, acl_capacity, sizeof(struct ACLStruct));
            if(acl_array == NULL){
                perror("ERR: Failed to reallocate memory mod_acl_element().\n");
                return false;
            }
        }
        acl_size++;
        return true;
    }else{                                          // Remove an element to the ACL structure
        if(acl_size == 0){                          // If already empty, do nothing.
            return true;
        }
        acl_size--;
        //Update size: typecasting     reallocation   pointer   new size   size of one ACLStruct
        acl_array = (struct ACLStruct *)realloc_struct(acl_array, acl_size, sizeof(struct ACLStruct));
        if(acl_array == NULL && acl_size != 0){
            perror("ERR: Failed to reallocate memory mod_acl_element().\n");
            return false;
        }
        return true;
    }
}

bool add_acl_data(char *fp, char *fs){
    if (fp == NULL || fs == NULL) {
        fprintf(stderr, "ERR: Null pointer passed to add_acl_data().\n");
        return false;
    }
    if(!mod_acl_element(true)){
        return false;
    }
    struct ACLStruct *element = &acl_array[acl_size - 1];
    if(element == NULL){
        fprintf(stderr, "ERR: Failed to add data to allocated memory in ACLStruct.\n");
        clear_acl();
        return false;
    }
    strncpy(acl_array->fp, fp, sizeof(acl_array->fp) - 1);
    acl_array->fp[sizeof(acl_array->fp) - 1] = '\0';
    strncpy(acl_array->fs, fs, sizeof(acl_array->fs) - 1);
    acl_array->fs[sizeof(acl_array->fs) - 1] = '\0';
    add_acl_time(acl_array);
    get_acl_data(acl_size-1);
    return true;
}

bool rem_acl_data(){
    return mod_acl_element(false);
}

void get_acl_data(int index){
    if(acl_array == NULL){
        fprintf(stderr, "ERR: Null pointer passed to get_acl_data().\n");
        return;
        
    }if(index < 0 || index >= acl_capacity){
        fprintf(stderr, "ERR: Index out of bounds in get_acl_data().\n");
        return;
    }
        printf("Filepath: %s.\n", acl_array[index].fp);
        printf("Filesystem: %s.\n", acl_array[index].fs);
}