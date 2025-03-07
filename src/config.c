#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"

void config_exists(){
    if(!path_exists("../config/uhb_config.txt")){
        printf("Configuration file does not exist. Creating...\n");
        FILE *file = fopen("../config/uhb_config.txt", "w");
        if (file) {
            fprintf(file, "This is the configuration file for UHB.\n");
            fclose(file);
        } else {
            printf("Error creating configuration file.\n");
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
        printf("Error adding command to configuration file.\n");
        return -1;
    }
}


