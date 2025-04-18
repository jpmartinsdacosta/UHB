#include <stdio.h>
#include <stdbool.h>

#include "menu.h"
#include "file.h"
#include "input.h"
#include "global_var.h"

/**
 * Functions specific to Access Control Lists:
 */

 const char *dicc_no_acl_fs[] = {         // Filesystems that do not support ACLs.
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


bool get_acl(){
    char path[MAX_FILEPATH_SIZE];
    char options[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
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

//bool set_acl() is OS-specific.

bool acl_incompatible_fs(char *fp){
    bool result = false;
    for(int i=0; i < get_diccionary_size(dicc_no_acl_fs) && !result; i++){
        result = find_strings_in_line(fp,dicc_no_acl_fs[i],"/etc/fstab");
    }
    return result;
}