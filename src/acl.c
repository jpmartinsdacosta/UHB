#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILE_PATH 200           // Maximum length of a file path.
#define MAX_CMD 300                 // Maximum length of a command.
#define MAX_OPTIONS_LENGTH 20       // Maximum length reserved for options to the user.
#define MAX_NAME 30                 // Maximum length of a username/groupname.

const char *incompatible_acl_fs[] = {
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

bool acl_incompatible_fs(char *filepath){
    bool result = false;
    for(int i=0; i < get_menu_size(incompatible_acl_fs) && !result; i++){
        result = find_strings_in_line(filepath,incompatible_acl_fs[i],"/etc/fstab");
    }
    return result;
}

void get_acl_common(){
    char path[MAX_FILE_PATH];
    char options[MAX_CMD];
    char command[MAX_CMD];
    if(get_filepath(path) && get_option(options)){
        printf("MSG: Press q to exit current view.\n");
        snprintf(command, sizeof(command), "getfacl \"%s\" \"%s\" | less", options, path);
        system(command);
        return true;
    }else{
        printf("ERR: ACL could not be retrieved.\n");
        return false;
    }
}

bool set_acl_common(){
    char path[MAX_FILE_PATH];
    char options[MAX_CMD];
    char command[MAX_CMD];
    if(get_filepath(path) && path_exists(path) && !acl_incompatible_fs(path)){
        get_user_input("MSG: Please enter setfacl options;",options,MAX_CMD);
        printf("MSG: Setting ACL...\n");
        snprintf(command, sizeof(command), "setfacl %s %s", options, path);
        add_config_command(command);
        return true;
    }else{
        printf("ERR: ACLs could not be set.\n");
        return false;
    }
}

