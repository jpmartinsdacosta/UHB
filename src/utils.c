#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

#include "menu.h"
#include "file.h"
#include "config.h"
#include "utils.h"
#include "input.h"
#include "os_interface.h"
#include "global_var.h"

#ifdef _WIN32
#include <io.h>
#include <windows.h>
const int OS = -1;
#elif defined (__FreeBSD__)
#include <unistd.h>
#include <sys/utsname.h>
const int OS = 0;
#elif defined (__linux__)
#include <unistd.h>
#include <sys/utsname.h>
const int OS = 1;
#else
const int OS = -1;
#endif

bool rc_local = false;              // Boolean to store if the rc.local file exists.

// TODO: Deprecate this function
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

bool sanitize_options(const char *input) {
    for (size_t i = 0; i < strlen(input); i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != '-' && input[i] != ' ') {
            printf("ERR: Non-sanitized options.\n");
            return false;
        }
    }
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