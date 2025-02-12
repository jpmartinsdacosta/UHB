#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"

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
    #ifdef defined __FreeBSD__
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

void get_user_input(char *path) {
    printf("Please enter the path to the file: ");
    // Read user input safely
    if (scanf("%255s", path) != 1) {  // Limit input size to avoid overflow
        printf("Error reading input.\n");
        path[0] = '\0';  // Set path to empty string to indicate error
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
            // Securely construct the command
            snprintf(command, sizeof(command), "ls -l -- \"%s\"", path);
    
            // Execute safely
            system(command);
        } else {
            printf("File does not exist.\n");
        }
    } 
}

void show_menu (){
    int choice = -1;
    exec_exists_common();
    while(choice != 0){
        printf("\nUHB Menu:\n");
        printf("1. Get DAC of a file.\n");
        if(option[0] == 1){
            printf("2. Get ACL of a file.\n");
        }
        if(option[1] == 1){
            printf("3. Configure firewall.\n");
        }
        if(option[2] == 1){
            printf("4. Configure logging.\n");
        }
        if(option[3] == 1){
            printf("5. Configure auditing.\n");
        }
        printf("0. Exit.\n");	
        printf("Please select an option: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                get_dac_common();
                break;
            case 2:
                printf("Option not implmented yet.\n");
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
            case 0:
                printf("\nGoodbye!\n");
                break;
            default:
                printf("Invalid option.\n");
                break;
        }
    }    
}