#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"
#include "file.h"
#include "config.h"
#include "utils.h"
#include "io.h"
#include "policy.h"
#include "os_interface.h"
#include "global_var.h"

bool config_modified = false;

/**
 * Configuration file functions
 */

bool is_conf_file_mod(){
    return config_modified;
}

bool reset_conf_file(){
    if(!path_exists(CONFIG_TEMPLATE_PATH)){
        printf("ERR: reset_conf_file(): Template does not exist.\n");
        return false;
    }else{
        copy_file(CONFIG_TEMPLATE_PATH,CONFIG_PATH);
        find_first_and_replace(CONFIG_PATH,"## uhb_os = NAN",get_os());
        clear_dac_array();
        return true;
    }
}

void conf_file_exists(){
    if(!path_exists(CONFIG_PATH)){
        printf("MSG: Configuration file does not exist. Creating...\n");
        if(reset_conf_file()){
            printf("MSG: Configuration file created successfully.\n");
        }else{
            printf("ERR: Error creating configuration file.\n");
        }
    }
}

bool add_conf_file(const char *command){
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

void view_conf_file(){
    if(!path_exists(CONFIG_PATH)){
        printf("ERR: Configuration file does not exist.\n");
    }else{
        printf("MSG: Press q to exit current view.\n");
        system("cat ../config/uhb_config.sh | less");
    }
}

bool apply_conf_file(){
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