#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"

#define LINE_MAX 256

bool set_initial_config(){
    FILE *file = fopen("../config/uhb_config.txt", "w");
    if (file) {
        fprintf(file, "## This is the configuration file for UHB.\n");
        fprintf(file, "## CONFIGURATION FILE PARAMETERS:\n");
        switch (os_detect()){
            case 0:
                fprintf(file, "uhb_os = BSD\n");
                break;
            case 1:
                fprintf(file, "uhb_os = DEB\n");
                break;
            case -1:
                fprintf(file, "uhb_os = NAN\n");
                break;
        }
        fprintf(file, "## CONFIGURATION FILE COMMANDS:\n");
        fclose(file);
        return true;
    } else {
        printf("ERR: Error clearing configuration file.\n");
        return false;
    }
}

void config_exists(){
    if(!path_exists("../config/uhb_config.txt")){
        printf("MSG: Configuration file does not exist. Creating...\n");
        if(set_initial_config()){
            printf("MSG: Configuration file created successfully.\n");
        }else{
            printf("ERR: Error creating configuration file.\n");
        }
    }
}

int add_config_command(char *command){
    FILE *file = fopen("../config/uhb_config.txt", "a");
    if (file) {
        fprintf(file, "%s\n", command);
        fclose(file);
        return 0;
    } else {
        printf("ERR: Error adding command to configuration file.\n");
        return -1;
    }
}

void view_config(){
    if(!path_exists("../config/uhb_config.txt")){
        printf("ERR: Configuration file does not exist.\n");
    }else{
        system("cat ../config/uhb_config.txt | less");
    }
}

