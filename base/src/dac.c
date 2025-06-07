#include <stdio.h>
#include <stdbool.h>

#include "file.h"
#include "config.h"
#include "utils.h"
#include "input_output.h"
#include "global_var.h"
#include "policy.h"

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Filepath to the configuration files to be used/edited in UHB.
#define UHB_DAC_CONFIG_CURRENT  "../config/current/dac.sh"

FlagCollection get_dac_fc, set_dac_fc;

static const char get_dac_flags[] = {
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

static const char set_dac_flags[] = {
    'c',    // like verbose but report only when a change is made
    'f',    // supress most error messages
    'v',    // verbose
    'R'     // default recursive
};

/**
 * Functions available to the user.
 */

void get_dac(){ 
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
        }
    }else{
        fprintf(stderr, "ERR: get_dac(): DAC could not be retrieved.\n");
    }
}

void set_dac(){
    char input[MAX_LINE_LENGTH];
    char path[MAX_FILEPATH_SIZE];
    char permission[6];
    char command[MAX_LINE_LENGTH];
    char user[MAX_NAME_LENGTH];
    char group[MAX_NAME_LENGTH];
    char flags[MAX_LINE_LENGTH];
    int opt = 1;
    init_flag(&set_dac_fc,4,set_dac_flags);
    while(opt == 1){
        get_user_input("MSG 1/5: Please enter the absolute filepath:", path, sizeof(path));
        get_user_input("MSG 2/5: Please enter the permission (e.g. 0777):", permission, 6);
        get_user_input("MSG 3/5: Please enter the target user:", user, MAX_NAME_LENGTH);
        get_user_input("MSG 4/5: Please enter the target group:", group, MAX_NAME_LENGTH);
        get_user_input("MSG 5/5: Please enter additional flags followed by a single '-':", flags, MAX_LINE_LENGTH);
        opt = three_option_input("MSG: Is the provided information correct? (Y)es/(N)o/E(x)it:",'Y','N','X');
    }
    if(opt == 0){
        if(path_exists(path) && check_permission(permission) && check_user(user) && check_group(group) && check_flags(flags,&set_dac_fc)){
            printf("MSG: Setting DAC...\n");
            add_dac_element(path,user,group,permission,is_recursive(flags));
            snprintf(command, sizeof(command), "chmod %s %s %s", flags, permission, path);
            append_to_file(command, UHB_DAC_CONFIG_CURRENT);
            snprintf(command, sizeof(command), "chown %s %s:%s %s\n", flags, user, group, path);
            append_to_file(command, UHB_DAC_CONFIG_CURRENT);
            return true;
        }else{
            fprintf(stderr, "ERR: set_dac(): DAC could not be set.\n");
        }
    } 
}

void rem_dac_entry() {
    remove_last_n_lines(UHB_DAC_CONFIG_CURRENT,3);
    rem_dac_element();
}

void view_dac_configuration() {
    view_file(UHB_DAC_CONFIG_CURRENT);
}

void reset_dac_configuration() {
    copy_file(UHB_SCRIPT_TEMPLATE_PATH,UHB_DAC_CONFIG_CURRENT);
}

void apply_dac_configuration() {
    char command[MAX_LINE_LENGTH];
    snprintf(command,sizeof(command),"sh %s",UHB_DAC_CONFIG_CURRENT);
    if(system(command) == 0){
        printf("MSG: DAC configuration applied successfully.\n");
    }else{
        fprintf(stderr,"ERR: Unable to apply DAC configuration.\n");
    }
}