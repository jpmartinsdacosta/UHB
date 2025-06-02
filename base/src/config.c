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
#include "module_var.h"
#include "global_var.h"

bool config_modified = false;

bool is_conf_file_mod(){
    return config_modified;
}

bool reset_uhb_conf(){
    if(!path_exists(CONFIG_TEMPLATE_PATH)){
        fprintf(stderr, "ERR: reset_uhb_conf(): UHB template configuration does not exist.\n");
        return false;
    }else{
        copy_file(CONFIG_TEMPLATE_PATH,CONFIG_UHB);
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

/**
 * Functions regarding the service configuration files at uhb/base/config/services/*
 */

void detect_execs(){
    find_exec_in_file("uhb_acl =",MODULE_PATH) ? printf("INI: ACL service detected.\n") : printf("INI: ACL service not found.\n");
    find_exec_in_file("uhb_mac =",MODULE_PATH) ? printf("INI: MAC service detected.\n") : printf("INI: MAC service not found.\n");
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
    snprintf(command, sizeof(command), "sh \"%s\"", UHB_DAC_CONFIG_CURRENT);
    system(command);
    printf("MSG: DAC policy applied.\n");
    if(acl_exists()){
        snprintf(command, sizeof(command), "sh \"%s\"", UHB_ACL_CONFIG_CURRENT);
        system(command);
        printf("MSG: ACL policy applied.\n");
    }
}

/**
 * Functions common to all
 */

bool reset_conf(){
    return (reset_uhb_conf() && reset_service_conf());
}
