#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "file.h"
#include "config.h"
#include "policy.h"
#include "global_var.h"

bool config_modified = false;

/**
 * Functions regarding the UHB Base Configuration file at uhb/base/config/files/config.sh
 */

bool is_conf_file_mod(){
    return config_modified;
}

bool reset_uhb_conf(){
    if(!path_exists(CONFIG_TEMPLATE_PATH)){
        fprintf(stderr, "ERR: reset_uhb_conf(): UHB template configuration does not exist.\n");
        return false;
    }else{
        copy_file(CONFIG_TEMPLATE_PATH,CONFIG_UHB);
        //find_first_and_replace(CONFIG_UHB,"## uhb_os = NAN", get_os()); // This function breaks and I don't know why.
        //clear_dac_array();
        return true;
    }
}

void uhb_conf_exists(const char *filepath){
    if(!path_exists(filepath)){
        printf("MSG: Configuration file does not exist. Creating...\n");
        if(reset_conf()){
            printf("MSG: Configuration file created successfully.\n");
        }else{
            fprintf(stderr, "ERR: uhb_conf_exists(): Error creating configuration file.\n");
        }
    }
}


bool add_uhb_command(const char *command){
    FILE *file = fopen(CONFIG_UHB, "a");
    if (file) {
        fprintf(file, "%s\n", command);
        fclose(file);
        config_modified = true;
        return true;
    } else {
        fprintf(stderr, "ERR: add_uhb_command(): Error adding command to configuration file.\n");
        return false;
    }
}

bool apply_uhb_conf(){
    FILE *file = fopen(CONFIG_UHB, "r");
    if(file != NULL){
        if(find_first_in_file(get_os(),CONFIG_UHB) != 4){
            fprintf(stderr, "ERR: apply_uhb_conf(): OS not found or not supported.\n");
            fclose(file);
            return false;
        }else{
            printf("MSG: Applying configuration file...\n");
            system("sh ../config/uhb_config.sh");
            fclose(file);
            return true;
        }
    }else{
        fprintf(stderr, "ERR: apply_uhb_conf(): Error applying configuration file.\n");
        return false;
    }
}

/**
 * Functions regarding the service configuration files at uhb/base/config/services/*
 */

bool reset_service_conf(){
    if(!path_exists(SERVICE_TEMPLATE_PATH)){
        fprintf(stderr, "ERR: reset_uhb_conf(): Service template configuration does not exist.\n");
        return false;
    }else{
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_DAC)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset DAC.\n");
            return false;
        }else{
            find_first_and_replace(CONFIG_DAC,"[service]", "DAC");
            return true;
        }
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_ACL)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset ACL.\n");
            return false;
        }else{
            find_first_and_replace(CONFIG_ACL,"[service]", "ACL");
            return true;
        }
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_MAC)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset MAC.\n");
            return false;
        }else{
            find_first_and_replace(CONFIG_MAC,"[service]", "MAC");
            return true;
        }
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_LOG)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset LOG.\n");
            return false;
        }else{
            find_first_and_replace(CONFIG_LOG,"[service]", "LOG");
            return true;
        }
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_AUD)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset AUD.\n");
            return false;
        }else{
            find_first_and_replace(CONFIG_AUD,"[service]", "AUD");
            return true;
        }
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_FWL)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset FWL.\n");
            return false;
        }else{
            find_first_and_replace(CONFIG_FWL,"[service]", "FWL");
            return true;
        }
        return true;
    }
}

bool add_service_command(const char *command, const char *filepath){
    FILE *file = fopen(filepath, "a");
    if (file) {
        fprintf(file, "%s\n", command);
        fclose(file);
        return true;
    } else {
        fprintf(stderr, "ERR: add_service_command(): Error adding command to the given configuration file.\n");
        return false;
    }
}

/**
 * Functions common to all
 */

bool reset_conf(){
    return (reset_uhb_conf() && reset_service_conf());
}

