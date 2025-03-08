#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "config.h"

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__FreeBSD__)
#include <unistd.h>
#include <sys/utsname.h>
#include "imp_bsd.h"
#elif defined (__linux__)
#include <unistd.h>
#include <sys/utsname.h>
#include "imp_deb.h"
#endif

// Array to store the menu options available to the user.
int option[4] = {0, 0, 0, 0};

// Boolean to store if the rc.local file exists.
bool rc_local = false;

int so_detect() {
    #ifdef _WIN32
    return -1;
    #else
    struct utsname uts;
    if (uname(&uts) == -1) {
        return -1;
    }
    if (strcmp(uts.sysname, "FreeBSD") == 0) {
        printf("FreeBSD OS detected.\n");
        return 0;
    } else if (strcmp(uts.sysname, "Linux") == 0) {
        printf("Debian OS detected.\n");
        return 1;
    }
    printf("Failed to detect or unsupported OS.\n");
    return -1;
    #endif
}

void exec_exists_common (){
    printf("DETECTED SUPPORTED PROGRAMS:\n");
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

void get_user_input(char *path) {
    printf("Please enter the path to the file: ");
    if (scanf("%255s", path) != 1) {
        printf("Error reading input.\n");
        path[0] = '\0';
    }
}

void get_dac_common() {
    char path[256];
    char command[300];

    get_user_input(path);
    if (path[0] == '\0') {
        return;
    } else {
        if (path_exists(path)) {
            snprintf(command, sizeof(command), "ls -l -- \"%s\"", path);
            system(command);
        } else {
            printf("File does not exist.\n");
        }
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

void set_dac_common() {
    char path[50];
    char test[50];
    char permission[4];
    char user[50];
    char group[50];
    char command[300];

    get_user_input(path);
    if (path[0] == '\0') {
        printf("Invalid path.\n");
        return;
    }
    if (path_exists(path)) {
        printf("Please enter file permissions in octal format (srwx): ");
        scanf("%4s", permission);
        if (!check_permission(permission)) {
            printf("Invalid permission format. Exiting.\n");
        }

        printf("Please enter user: ");
        scanf("%50s", user);
        printf("Please enter group: ");
        scanf("%50s", group);
        if (!check_ug_common(user) || !check_ug_common(group)) {
            printf("Invalid user and/or group. Exiting.\n");
        }else{
            snprintf(command, sizeof(command), "chown %s %s", user, path);
            add_config_command(command);
            snprintf(command, sizeof(command), "chgrp %s %s", group, path);
            add_config_command(command);
            snprintf(command, sizeof(command), "chmod %s %s\n", permission, path);
            add_config_command(command);
            printf("DAC added to config file.\n");
        }
    } else {
        printf("File does not exist.\n");
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