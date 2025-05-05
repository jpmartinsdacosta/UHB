#include <stdio.h>
#include <stdbool.h>

#include "file.h"
#include "config.h"
#include "utils.h"
#include "input.h"
#include "global_var.h"
#include "policy.h"

Flag get_dac_opt[] = {
    {'a',false,false},    // Include entries with .
    {'l',false,false},    // Long listing format
    {'h',false,false},    // With -l print human-readable sizes
    {'R',false,true},    // Recursive
    {'r',false,false},    // Reverse
    {'t',false,false},    // Sort by time
    {'S',false,false},    // Sort by size
    {'1',false,false},    // List 1 file per line
    {'d',false,false}     // List directories
};

FlagList get_dac_lst = {get_dac_opt,sizeof(get_dac_opt)/sizeof(get_dac_opt[0])};

bool get_dac(){ 
    char path[MAX_FILEPATH_SIZE];
    char options[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    if(get_filepath(path) && get_option(options,&get_dac_lst)){
        printf("MSG: Press q to exit current view.\n");
        snprintf(command, sizeof(command), "ls \"%s\" -- \"%s\" | less", options, path);
        system(command);
        return true;
    }else{
        printf("ERR: DAC could not be retrieved.\n");
        return false;
    }
}

Flag set_dac_opt[] = {
    {'c',false,false},    // like verbose but report only when a change is made
    {'f',false,false},    // supress most error messages
    {'v',false,false},    // verbose
    {'R',false,true}     // default recursive
};

FlagList set_dac_lst = {set_dac_opt,sizeof(set_dac_opt)/sizeof(set_dac_opt[0])};

bool set_dac(){
    char path[MAX_FILEPATH_SIZE];
    char permission[6];                 // Needed for /n and /0?
    char command[MAX_LINE_LENGTH];
    char user[MAX_NAME_LENGTH];
    char group[MAX_NAME_LENGTH];
    char options[MAX_LINE_LENGTH];

    if(get_filepath(path) && path_exists(path) && get_option(options,&set_dac_lst)){
        get_user_input("MSG: Please enter the permission (e.g. 0777):", permission, 6);
        get_user_input("MSG: Please enter the target user:", user, MAX_NAME_LENGTH);
        get_user_input("MSG: Please enter the target group:", group, MAX_NAME_LENGTH);
        if(check_permission(permission) && check_user(user) && check_group(group)){
            if(add_dac_element(path,user,group,permission)){
                printf("MSG: Setting DAC...\n");
                snprintf(command, sizeof(command), "chmod %s %s %s", options, permission, path);
                add_conf_file(command);
                snprintf(command, sizeof(command), "chown %s %s:%s %s\n", options, user, group, path);
                add_conf_file(command);
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