#include <stdio.h>
#include <stdbool.h>

#include "file.h"
#include "config.h"
#include "utils.h"
#include "input.h"
#include "global_var.h"
#include "perms.h"

 bool get_dac(){ 
    char path[MAX_FILEPATH_SIZE];
    char options[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    if(get_filepath(path) && get_option(options)){
        printf("MSG: Press q to exit current view.\n");
        snprintf(command, sizeof(command), "ls \"%s\" -- \"%s\" | less", options, path);
        system(command);
        return true;
    }else{
        printf("ERR: DAC could not be retrieved.\n");
        return false;
    }
}

bool set_dac(){
    char path[MAX_FILEPATH_SIZE];
    char permission[6];                 // Needed for /n and /0?
    char command[MAX_LINE_LENGTH];
    char user[MAX_NAME_LENGTH];
    char group[MAX_NAME_LENGTH];
    char options[MAX_LINE_LENGTH];

    if(get_filepath(path) && path_exists(path) && get_option(options)){
        get_user_input("MSG: Please enter the permission (e.g. 0777):", permission, 6);
        get_user_input("MSG: Please enter the target user:", user, MAX_NAME_LENGTH);
        get_user_input("MSG: Please enter the target group:", group, MAX_NAME_LENGTH);
        if(check_permission(permission) && check_user(user) && check_group(group)){
            if(add_dac_element(path,user,group,permission)){
                printf("MSG: Setting DAC...\n");
                snprintf(command, sizeof(command), "chmod %s %s %s", options, permission, path);
                add_config_command(command);
                snprintf(command, sizeof(command), "chown %s %s:%s %s\n", options, user, group, path);
                add_config_command(command);
                return true;
            }else{
                return false;
            }
        }else{
            printf("ERR: DAC could not be set.\n");
            return false;
        }
    }else{
        printf("ERR: Invalid/non-existent path.\n");
        return false;
    }
}