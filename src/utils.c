#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "config.h"

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

#define MAX_FILE_PATH 256       // Maximum length of a file path.
#define MAX_CMD 300             // Maximum length of a command.
#define MAX_NAME 50             // Maximum length of a username/groupname.

// Array to store the menu options available to the user.
int option[4] = {0, 0, 0, 0};

// Boolean to store if the rc.local file exists.
bool rc_local = false;

int so_detect(int OS){
    switch (OS) {
        case 1:
            printf("MSG: FreeBSD detected.\n");
            return 0;
        case 2:
            printf("MSG: Debian detected.\n");
            return 1;
        default:
            printf("MSG: Unsupported OS.\n");
            return -1;
    }
}

void exec_exists_common (){
    printf("MSG: Detected supported programs:\n");
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
        return false;
    }
}

void rc_local_exists_common(){
    rc_local = path_exists("/etc/rc.local");
}

bool get_user_input(char *path){
    printf("MSG: Please enter the path to the file:");
    if (scanf("%255s", path) != 1) {
        printf("ERR: Error reading input.\n");
        return false;
    }
    return true;
}

bool get_dac_common(){ 
    char path[MAX_FILE_PATH];
    char command[MAX_CMD];
    if(get_user_input(path) && path_exists(path)){
        snprintf(command, sizeof(command), "ls -l -- \"%s\" | less", path);
        system(command);
        return true;
    }else{
        printf("ERR: Invalid/non existent path.\n");
        return false;
    }
}

bool check_permission(char *permission){
    if (strlen(permission) != 4) {
        return false;
    }
    for (int i = 0; i < 4; i++) {
        if (permission[i] < '0' || permission[i] > '7') {
            return false;
        }
    }
    return true;
}

bool check_ug_common(char *target){
    bool result = false;
    if (strlen(target) == 0) {
        return false;
    }
    #ifdef __FreeBSD__
        result = check_ug_bsd(target);
    #elif defined (__linux__)
        result = check_ug_deb(target);
    #endif
    return result;
}

void set_dac_common(){
    char path[MAX_FILE_PATH];
    char permission[5];
    char command[MAX_CMD];
    char user[MAX_NAME];
    char group[MAX_NAME];

    if(get_user_input(path) && path_exists(path)){
        printf("MSG: Please enter the permission (e.g. 0777):");
        if (scanf("%4s", permission) != 1 || !check_permission(permission)) {
            printf("ERR: Invalid permissions set.\n");
            return;
        }
        printf("MSG: Please enter the target user:");
        if (scanf("%50s", user) != 1 || !check_ug_common(user)) {
            printf("ERR: Invalid/non existent user.\n");
            return;
        }
        printf("MSG: Please enter the target group:");
        if (scanf("%50s", group) != 1 || !check_ug_common(group)) {
            printf("ERR: Invalid/non existent user.\n");
            return;
        }
        printf("MSG: Setting DAC...\n");
        snprintf(command, sizeof(command), "chmod %s %s", permission, path);
        add_config_command(command);
        snprintf(command, sizeof(command), "chown %s:%s %s\n", user, group, path);
        add_config_command(command);
    }else{
        printf("ERR: Invalid/non existent path.\n");
    }
}

void show_menu (){
    int choice = -1;
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
        printf("8. Apply changes to system from configuration file.\n");
        printf("9. Clear configuration file.\n");
        printf("0. Exit.\n");	
        printf("Please select an option: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                get_dac_common();
                break;
            case 2:
                set_dac_common();
                break;
            case 3:
                printf("Option not implmented yet.\n");
                break;
            case 4:
                printf("Option not implmented yet.\n");
                break;
            case 5: 
                printf("Option not implmented yet.\n");
                break;
            case 6:
                printf("Option not implmented yet.\n");
                break;
            case 7:
                view_config();
                break;
            case 8:
                printf("Option not implmented yet.\n");
                break;
            case 9:
                clear_config();
                break;
            case 0:
                printf("\nGoodbye!\n");
                break;
            default:
                printf("Invalid option.\n");
                break;
        }
    }    
}