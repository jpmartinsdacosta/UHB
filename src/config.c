#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_MAX 256

bool config_modified = false;

bool is_config_modified(){
    return config_modified;
}

bool set_initial_config(){
    FILE *file = fopen("../config/uhb_config.sh", "w");
    if (file) {
        fprintf(file, "#!/bin/sh\n");
        fprintf(file, "## This is the configuration file for UHB.\n");
        fprintf(file, "## CONFIGURATION FILE PARAMETERS:\n");
        switch (os_detect()){
            case 0:
                fprintf(file, "##uhb_os = BSD\n");
                break;
            case 1:
                fprintf(file, "##uhb_os = DEB\n");
                break;
            case -1:
                fprintf(file, "##uhb_os = NAN\n");
                break;
        }
        fprintf(file, "## CONFIGURATION FILE COMMANDS:\n");
        fclose(file);
        config_modified = false;                                            // Set config modification to false
        return true;
    } else {
        printf("ERR: Error clearing configuration file.\n");
        return false;
    }
}

void config_exists(){
    if(!path_exists("../config/uhb_config.sh")){
        printf("MSG: Configuration file does not exist. Creating...\n");
        if(set_initial_config()){
            printf("MSG: Configuration file created successfully.\n");
        }else{
            printf("ERR: Error creating configuration file.\n");
        }
    }
}

bool add_config_command(char *command){
    FILE *file = fopen("../config/uhb_config.sh", "a");
    if (file) {
        fprintf(file, "%s\n", command);
        fclose(file);
        config_modified = true;                                             // Set config modification to true
        return true;
    } else {
        printf("ERR: Error adding command to configuration file.\n");
        return false;
    }
}

void view_config(){
    if(!path_exists("../config/uhb_config.sh")){
        printf("ERR: Configuration file does not exist.\n");
    }else{
        printf("MSG: Press q to exit current view.\n");
        system("cat ../config/uhb_config.sh | less");
    }
}

int find_first_config(char *target){
    FILE *file = fopen("../config/uhb_config.sh", "r");
    char line[LINE_MAX];
    int position = 0;
    bool found = false;
    if (file) {
        while (fgets(line, LINE_MAX, file)) {
            if (strstr(line, target)) {
                break;
            }
            position += !found;                     
        }
        fclose(file);
        return position;
    }else{
        printf("ERR: Error reading configuration file.\n");
        return -1;
    }
}

void user_find_first_config(){
    char target[LINE_MAX];
    int answer = -1;
    get_user_input("MSG: Test search config file:",target,LINE_MAX);
    answer = find_first_config(target);
    if(answer != -1){
        printf("MSG: Found %s at line %d in configuration file.\n",target, answer);
    }
}

bool apply_config(int os){
    FILE *file = fopen("../config/uhb_config.sh", "r");
    if(file){
        switch(os){
            case 0:
                if(find_first_config("##uhb_os = BSD") != 4){         // Line must be 4, for more details check header info.
                    printf("MSG: Applying BSD configuration.\n");
                    system("bash ../config/uhb_config.sh");
                    return true;
                }else{
                    printf("ERR: BSD configuration not found.\n");
                    return false;
                }
                break;
            case 1:
                if(find_first_config("##uhb_os = DEB") != 4){         // Line must be 4, for more details check header info.
                    printf("MSG: Applying DEB configuration.\n");
                    system("bash ../config/uhb_config.sh");
                    return true;
                }else{
                    printf("ERR: DEB configuration not found.\n");
                    return false;
                }
                break;
            default:
                printf("ERR: OS not found or not supported.\n");
                return false;
                break;
        }
    }else{
        printf("ERR: Error applying configuration file.\n");
        return false;
    }
}