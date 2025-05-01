#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"
#include "file.h"
#include "config.h"
#include "utils.h"
#include "input.h"
#include "os_interface.h"
#include "global_var.h"

#ifdef _WIN32
extern const int OS = -1;
#elif defined (__FreeBSD__)
#include <unistd.h>
#include <sys/utsname.h>
extern const int OS = 0;
#elif defined (__linux__)
#include <unistd.h>
#include <sys/utsname.h>
extern const int OS = 1;
#else
extern const int OS = -1;
#endif

bool rc_local = false;              // Boolean to store if the rc.local file exists.

int os_detect(){
    int ans = -1;
    switch (OS) {
        case 0:
            printf("INI: FreeBSD detected.\n");
            ans = 0;
            break;
        case 1:
            printf("INI: Debian detected.\n");
            ans = 1;
            break;
        case -1:
            printf("INI: Unsupported OS.\n");
            ans = -1;
            break;
        default:
            ans = -1;
            break;
    }  
    return ans;
}

bool sanitize_name(const char *input) {
    for (size_t i = 0; i < strlen(input); i++) {
        if (!isalnum(input[i])) {
            printf("ERR: Non-sanitized name.\n");
            return false;
        }
    }
    return true;
}

bool check_user(const char *user){
    char command[MAX_LINE_LENGTH];
    if (sanitize_name(user)){
        snprintf(command, sizeof(command), "getent passwd \"%s\" >/dev/null 2>&1", user);
    }else{
        printf("ERR: Invalid user.\n");
        return false;
    }
    if(system(command) == 0){
        return true;
    }else{
        return false;
    }
}

bool check_group(const char *group){
    char command[MAX_LINE_LENGTH];
    if (sanitize_name(group)){
        snprintf(command, sizeof(command), "getent group \"%s\" >/dev/null 2>&1", group);
    }else{
        printf("ERR: Invalid group.\n");
        return false;
    }
    if(system(command) == 0){
        return true;
    }else{
        return false;
    }
}

void list_avail_flags(FlagList *list){
    printf("Available options: [-");
    for (int i = 0; i < list->size; i++) {
        printf("%c",list->flags);
    }
    printf("]\n");
}

void reset_flags_list(FlagList *list) {
    for (int i = 0; i < list->size; i++) {
        list->flags[i].used = false;
    }
}

bool validate_options(const char *input, FlagList *list) {
    if (input[0] != '-' || input[1] == '\0')
        return false;
    reset_flags_list(list);
    for (int i = 1; input[i] != '\0'; i++) {
        char current_char = input[i];
        bool found = false;
        for (int j = 0; j < list->size; j++) {
            if (current_char == list->flags[j].flag && !list->flags[j].used) {
                list->flags[j].used = true;
                found = true;
            }
        }
        if (!found) {
            reset_flags_list(list);
            return false;
        }
    }
    reset_flags_list(list);
    return true;
}

bool check_permission(const char *permission){
    if (strlen(permission) != 4) {
        printf("ERR: Invalid permissions set.\n");
        return false;
    }
    for (int i = 0; i < 4; i++) {
        if (permission[i] < '0' || permission[i] > '7') {
            printf("ERR: Invalid permissions set.\n");
            return false;
        }
    }
    return true;
}

