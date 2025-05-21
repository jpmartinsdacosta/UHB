#include <stdio.h>
#include <stdbool.h>

#include "file.h"
#include "config.h"
#include "utils.h"
#include "input_output.h"
#include "global_var.h"
#include "policy.h"

FlagCollection get_dac_fc, set_dac_fc;

const char get_dac_flags[] = {
    'a',    // Include entries with .
    'l',    // Long listing format
    'h',    // With -l print human-readable sizes
    'R',    // Recursive
    'r',    // Reverse
    't',    // Sort by time
    'S',    // Sort by size
    '1',    // List 1 file per line
    'd',    // List directories
};

const char set_dac_flags[] = {
    'c',    // like verbose but report only when a change is made
    'f',    // supress most error messages
    'v',    // verbose
    'R'     // default recursive
};

bool get_dac(){ 
    char path[MAX_FILEPATH_SIZE];
    char flags[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    init_flag(&get_dac_fc,9,get_dac_flags);
    if(get_filepath(path)){
        get_user_input("MSG: Please enter additional flags followed by a single '-':", flags, MAX_LINE_LENGTH);
        if(check_flags(flags,&get_dac_fc)){
            printf("MSG: Press q to exit current view.\n");
            snprintf(command, sizeof(command), "ls \"%s\" \"%s\" | less", flags, path);
            system(command);
            return true;
        }
        return false;
    }else{
        fprintf(stderr, "ERR: get_dac(): DAC could not be retrieved.\n");
        return false;
    }
}

bool set_dac(){
    char path[MAX_FILEPATH_SIZE];
    char permission[6];
    char command[MAX_LINE_LENGTH];
    char user[MAX_NAME_LENGTH];
    char group[MAX_NAME_LENGTH];
    char flags[MAX_LINE_LENGTH];
    init_flag(&set_dac_fc,4,set_dac_flags);
    if(get_filepath(path)){
        get_user_input("MSG: Please enter the permission (e.g. 0777):", permission, 6);
        get_user_input("MSG: Please enter the target user:", user, MAX_NAME_LENGTH);
        get_user_input("MSG: Please enter the target group:", group, MAX_NAME_LENGTH);
        get_user_input("MSG: Please enter additional flags followed by a single '-':", flags, MAX_LINE_LENGTH);
        if(check_permission(permission) && check_user(user) && check_group(group) && check_flags(flags,&set_dac_fc)){
            printf("MSG: Setting DAC...\n");
            add_dac_element(path,user,group,permission,is_recursive(flags));
            snprintf(command, sizeof(command), "chmod %s %s %s", flags, permission, path);
            add_service_command(command, CONFIG_DAC);
            snprintf(command, sizeof(command), "chown %s %s:%s %s\n", flags, user, group, path);
            add_service_command(command, CONFIG_DAC);
            return true;
        }else{
            fprintf(stderr, "ERR: set_dac(): DAC could not be set.\n");
            return false;
        }
    }else{
        printf("ERR: Invalid/non-existent path.\n");
        return false;
    }
}