#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"
#include "file.h"
#include "config.h"
#include "utils.h"
#include "input.h"
#include "perms.h"
#include "os_interface.h"
#include "global_var.h"

#define CONFIG_PATH "../config/uhb_config.sh"
#define TEMPLATE_PATH "../config/template/uhb_config_template.txt"

bool config_modified = false;

bool is_config_modified(){
    return config_modified;
}

bool reset_config(){
    if(!path_exists(TEMPLATE_PATH)){
        printf("ERR: reset_config(): Template does not exist.\n");
        return false;
    }else{
        copy_file(TEMPLATE_PATH,CONFIG_PATH);
        find_first_and_replace(CONFIG_PATH,"##uhb_os = NAN",get_os());
        return true;
    }
}

void config_exists(){
    if(!path_exists(CONFIG_PATH)){
        printf("MSG: Configuration file does not exist. Creating...\n");
        if(reset_config()){
            printf("MSG: Configuration file created successfully.\n");
        }else{
            printf("ERR: Error creating configuration file.\n");
        }
    }
}

bool add_config_command(const char *command){
    FILE *file = fopen(CONFIG_PATH, "a");
    if (file) {
        fprintf(file, "%s\n", command);
        fclose(file);
        config_modified = true;
        return true;
    } else {
        printf("ERR: Error adding command to configuration file.\n");
        return false;
    }
}

void view_config(){
    if(!path_exists(CONFIG_PATH)){
        printf("ERR: Configuration file does not exist.\n");
    }else{
        printf("MSG: Press q to exit current view.\n");
        system("cat ../config/uhb_config.sh | less");
    }
}

void user_find_first_config(){
    char target[MAX_LINE_LENGTH];
    int answer = -1;
    get_user_input("MSG: Test search config file:",target,MAX_LINE_LENGTH);
    answer = find_first_in_file(target,CONFIG_PATH);
    if(answer != -1){
        printf("MSG: Found %s at line %d in configuration file.\n",target, answer);
    }
}

bool apply_config(){
    FILE *file = fopen(CONFIG_PATH, "r");
    if(file != NULL){
        if(find_first_in_file(get_os(),CONFIG_PATH) != 4){
            printf("ERR: OS not found or not supported.\n");
            fclose(file);
            return false;
        }else{
            printf("MSG: Applying configuration file...\n");
            system("sh ../config/uhb_config.sh");
            fclose(file);
            return true;
        }
    }else{
        printf("ERR: Error applying configuration file.\n");
        return false;
    }
}