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


