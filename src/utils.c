#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

#include "utils.h"
#include "config.h"
#include "input.h"

#ifdef _WIN32
#include <io.h>
#include <windows.h>
const int OS = -1;
#elif defined (__FreeBSD__)
#include <unistd.h>
#include <sys/utsname.h>
#include "imp_bsd.h"
const int OS = 0;
#elif defined (__linux__)
#include <unistd.h>
#include <sys/utsname.h>
#include "imp_deb.h"
const int OS = 1;
#else
const int OS = -1;
#endif

#define MAX_FILE_PATH 200       // Maximum length of a file path.
#define MAX_CMD 300             // Maximum length of a command.
#define MAX_OPTIONS_LENGTH 20   // Maximum length reserved for options to the user.
#define MAX_NAME 30             // Maximum length of a username/groupname.

// Array to store the menu options available to the user.
int option[4] = {0, 0, 0, 0};

// Boolean to store if the rc.local file exists.
bool rc_local = false;

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

void exec_exists_common (){
    printf("INI: Detected supported programs:\n");
    #ifdef __FreeBSD__
        exec_exists_bsd(option);
    #elif defined (__linux__)
        exec_exists_deb(option);
    #endif  
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

bool get_dac_common(){ 
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

bool check_ug_common(char *target){
    char command[MAX_CMD];
    if (sanitize_name(target)){
        snprintf(command, sizeof(command), "id -u \"%s\" >/dev/null 2>&1", target);
    }else{
        printf("ERR: Invalid user/group.\n");
        return false;
    }
    if(system(command) == 0){
        return true;
    }else{
        return false;
    }
}

bool set_dac_common(){
    char path[MAX_FILE_PATH];
    char permission[6];                 // For some reason, this works only if size = 6. Go figure.
    char command[MAX_CMD];
    char user[MAX_NAME];
    char group[MAX_NAME];
    char options[MAX_CMD];

    if(get_filepath(path) && path_exists(path) && get_option(options)){
        get_user_input("MSG: Please enter the permission (e.g. 0777):", permission, 6);
        get_user_input("MSG: Please enter the target user:", user, MAX_NAME);
        get_user_input("MSG: Please enter the target group:", group, MAX_NAME);
        if(check_permission(permission) && check_ug_common(user) && check_ug_common(group)){
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

void show_menu (){
    int choice = -1;
    char input[3]; // Buffer to store user input
    system("clear");
    exec_exists_common();
    while(choice != 0){
        printf("\nUHB Menu:\n");
        printf("1. Get DAC of a file.\n");
        printf("2. Set DAC of a file.\n");
        if(option[0] == 1){ printf("3. Set ACL of a file.\n"); }
        if(option[1] == 1){ printf("4. Configure firewall.\n"); }
        if(option[2] == 1){ printf("5. Configure logging.\n"); }
        if(option[3] == 1){ printf("6. Configure auditing.\n"); }
        printf("7. View configuration file.\n");
        printf("8. Apply changes to configuration file.\n");
        printf("9. Clear configuration file.\n");
        printf("0. Exit.\n");
        if(get_user_input("Please select an option: ", input, sizeof(input)) == -1){
            continue;
        }
        choice = atoi(input); // Convert input to integer
        switch(choice){
            case 1:
                system("clear");
                get_dac_common();
                break;
            case 2:
                system("clear");
                set_dac_common();
                break;
            case 3:
                system("clear");
                printf("MSG: Option not implemented yet.\n");
                break;
            case 4:
                system("clear");
                printf("MSG: Option not implemented yet.\n");
                break;
            case 5:
                system("clear"); 
                printf("MSG: Option not implemented yet.\n");
                break;
            case 6:
                system("clear");
                printf("MSG: Option not implemented yet.\n");
                break;
            case 7:
                system("clear");
                printf("MSG: Press q to exit current view.\n");
                view_config();
                break;
            case 8:
                system("clear");
                apply_config(OS);
                break;
            case 9:
                system("clear");
                set_initial_config();
                break;
            case 0:
                system("clear");
                printf("\nGoodbye!\n");
                break;
            default:
                system("clear");
                printf("MSG: Invalid option.\n");
                break;
        }
    }    
}