#include <stdio.h>
#include <stdbool.h>

#include "menu.h"
#include "file.h"
#include "io.h"
#include "policy.h"
#include "global_var.h"
#include "imp_bsd.h"

const char *no_acl_fs[] = {
    "tmpfs",
    "vfat",
    "exfat",
    "iso9660",
    "squashfs",
    "msdosfs",
    "procfs",
    "sysfs",
    NULL
};


/**
 * Functions specific to Access Control Lists:
 */

Flag get_acl_opt[] = {
    {'d',false,false},    // Operation applies to the default ACL instead of the access ACL
    {'n',false,false},    // Display numeric user and group IDs 
};

FlagList get_acl_lst = {get_acl_opt,sizeof(get_acl_opt)/sizeof(get_acl_opt[0])};

bool get_acl(){
    char path[MAX_FILEPATH_SIZE];
    char options[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    if(get_filepath(path) && get_option(options,&get_acl_opt)){
        printf("MSG: Press q to exit current view.\n");
        snprintf(command, sizeof(command), "getfacl \"%s\" \"%s\" | less", options, path);
        system(command);
        return true;
    }else{
        printf("ERR: ACL could not be retrieved.\n");
        return false;
    }
}

bool is_acl_enabled_bsd(const char* fp){
    char *filesystem = find_fs_from_path(fp);
    if (filesystem == NULL) {
        printf("ERR: Unable to find the filesystem for the given file path.\n");
        return false;
    }
    bool result = find_strings_in_line(filesystem, "acls", "/etc/fstab");
    free(filesystem);
    return result;
}

bool acl_incompatible_fs(char *fp){
    bool result = false;
    for(int i=0; i < get_diccionary_size(no_acl_fs) && !result; i++){
        result = find_strings_in_line(fp,no_acl_fs[i],"/etc/fstab");
    }
    return result;
}

bool set_acl(){
    char path[MAX_FILEPATH_SIZE];
    char options[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    get_filepath(path);
    if(!is_acl_enabled_bsd(path)){
        printf("ERR: ACLs are not enabled in the filesystem of the given file.\n");
        printf("ERR: Make sure the enable them in your BSD system visa fstab.\n");
        return false;
    }else{
        if(path_exists(path) && !acl_incompatible_fs(path)){
            get_user_input("MSG: Please enter setfacl options;",options,MAX_LINE_LENGTH);
            printf("MSG: Setting ACL...\n");
            snprintf(command, sizeof(command), "setfacl %s %s", options, path);
            add_uhb_command(command);
            return true;
        }else{
            printf("ERR: ACLs could not be set.\n");
            return false;
        }
    }
}

