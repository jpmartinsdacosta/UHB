#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "acl.h"
#include "mac.h"
#include "log.h"
#include "aud.h"
#include "fwl.h"
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
        //find_first_and_replace(CONFIG_UHB,"## uhb_os = NAN", get_os()); DOES NOT WORK...
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
    char command[MAX_LINE_LENGTH];
    if(file != NULL){
        if(find_first_in_file(get_os(),CONFIG_UHB) != 4){
            fprintf(stderr, "ERR: apply_uhb_conf(): OS not found or not supported.\n");
            fclose(file);
            return false;
        }else{
            printf("MSG: Applying configuration file...\n");
            snprintf(command, sizeof(command), "sh \"%s\"", CONFIG_UHB);
            system(command);
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
        }
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_ACL)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset ACL.\n");
            return false;
        }
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_MAC)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset MAC.\n");
            return false;
        }
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_LOG)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset LOG.\n");
            return false;
        }
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_AUD)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset AUD.\n");
            return false;
        }
        if(!copy_file(SERVICE_TEMPLATE_PATH,CONFIG_FWL)){
            fprintf(stderr, "ERR: reset_uhb_conf(): Failed to reset FWL.\n");
            return false;
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

void apply_service_conf() {
    char command[MAX_LINE_LENGTH];
    snprintf(command, sizeof(command), "sh \"%s\"", CONFIG_DAC);
    system(command);
    printf("MSG: DAC policy applied.\n");
    if(acl_exists()){
        snprintf(command, sizeof(command), "sh \"%s\"", CONFIG_ACL);
        system(command);
        printf("MSG: ACL policy applied.\n");
    }
    if(mac_exists()){
        snprintf(command, sizeof(command), "sh \"%s\"", CONFIG_MAC);
        system(command);
        printf("MSG: MAC policy applied.\n");
    }
    if(log_exists()){
        snprintf(command, sizeof(command), "sh \"%s\"", CONFIG_LOG);
        system(command);
        printf("MSG: Logging policy applied.\n");
    }
    if(aud_exists()){
        snprintf(command, sizeof(command), "sh \"%s\"", CONFIG_AUD);
        system(command);
        printf("MSG: Auditing policy applied.\n");
    }
    if(fwl_exists()){
        snprintf(command, sizeof(command), "sh \"%s\"", CONFIG_FWL);
        system(command);
        printf("MSG: Firewall policy applied.\n");
    }
}

/**
 * Functions common to all
 */

bool reset_conf(){
    return (reset_uhb_conf() && reset_service_conf());
}

void apply_conf(){
    apply_service_conf();
    apply_uhb_conf();
}

