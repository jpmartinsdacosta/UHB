#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "os_interface.h"
#include "global_var.h"
#include "file.h"

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
            fprintf(stderr, "ERR: sanitize_name(): Non-sanitized name.\n");
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
        fprintf(stderr, "ERR: check_user(): Invalid user.\n");
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
        fprintf(stderr, "ERR: check_group(): Invalid group.\n");
        return false;
    }
    if(system(command) == 0){
        return true;
    }else{
        return false;
    }
}

bool check_permission(const char *permission){
    if (strlen(permission) != 4) {
        fprintf(stderr, "ERR: check_permission(): Invalid permissions set.\n");
        return false;
    }
    for (int i = 0; i < 4; i++) {
        if (permission[i] < '0' || permission[i] > '7') {
            fprintf(stderr, "ERR: check_permission(): Invalid permissions set.\n");
            return false;
        }
    }
    return true;
}

bool exec_exists(const char *exec){
    char command[MAX_LINE_LENGTH];
    snprintf(command, sizeof(command), "command -v \"%s\" > /dev/null 2>&1", exec);
    if(system(command) == 0){
        return true;
    }else{
        return false;
    }
}

bool is_contained(const char* path1, const char* path2) {
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);
    if (len1 > len2) {
        return false;
    }
    if (strncmp(path1, path2, len1) != 0) {
        return false;
    }
    if (len1 == len2 || path2[len1] == '/') {
        return true;
    }
    return false;
}

bool is_recursive(const char *flags){
    for(int i = 0; i < strlen(flags); i++){
        if(tolower(flags[i]) == 'r'){
            return true;
        }
    }
    return false;
}

bool is_port_open(int port) {
    char command[256];
    snprintf(command, sizeof(command), "ss -tuln | grep '%d ' | grep 'LISTEN' >/dev/null 2>&1", port);
    if(system(command) == 0){
        return true;
    }else{
        return false;
    }
}

