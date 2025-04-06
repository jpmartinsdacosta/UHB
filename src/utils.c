#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

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

/**
 * @note TO DO LIST: Create a function that searches for incompatible fstab filesystems for acl 
 * Then create and test get_acl_bsd. If it works, proceed with get_acl_deb.
 * 
 * If all goes well, create a new config file or segment to add acl entires.
 * Then create set_acl_bsd and deb respectively.
 * */ 

#define MAX_FILE_PATH 200           // Maximum length of a file path.
#define MAX_CMD 300                 // Maximum length of a command.
#define MAX_OPTIONS_LENGTH 20       // Maximum length reserved for options to the user.
#define MAX_NAME 30                 // Maximum length of a username/groupname.

bool rc_local = false;              // Boolean to store if the rc.local file exists.

int os_detect(){
    switch (OS) {
        case 0:
            printf("INI: FreeBSD detected.\n");
            return 0;
        case 1:
            printf("INI: Debian detected.\n");
            return 1;
        case -1:
            printf("INI: Unsupported OS.\n");
            return -1;
    }
}

bool sanitize_name(char *input) {
    for (int i = 0; i < strlen(input); i++) {
        if (!isalnum(input[i])) {
            printf("ERR: Non-sanitized name.\n");
            return false;
        }
    }
    return true;
}

bool sanitize_options(char *input) {
    for (int i = 0; i < strlen(input); i++) {
        if (!isalnum(input[i]) && !input[i] == '-' && !input[i] == ' ') {
            printf("ERR: Non-sanitized options.\n");
            return false;
        }
    }
    return true;
}

bool path_exists(char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return true;
    } else {
        printf("MSG: File path %s does not exist.\n", path);
        return false;
    }
}

void rc_local_exists_common(){
    rc_local = path_exists("/etc/rc.local");
}

bool get_dac(){ 
    char path[MAX_FILE_PATH];
    char options[MAX_CMD];
    char command[MAX_CMD];
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

bool check_permission(char *permission){
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

bool check_user(char *user){
    char command[MAX_CMD];
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

bool check_group(char *group){
    char command[MAX_CMD];
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

bool set_dac(){
    char path[MAX_FILE_PATH];
    char permission[6];                 // Needed for /n and /0?
    char command[MAX_CMD];
    char user[MAX_NAME];
    char group[MAX_NAME];
    char options[MAX_CMD];

    if(get_filepath(path) && path_exists(path) && get_option(options)){
        get_user_input("MSG: Please enter the permission (e.g. 0777):", permission, 6);
        get_user_input("MSG: Please enter the target user:", user, MAX_NAME);
        get_user_input("MSG: Please enter the target group:", group, MAX_NAME);
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