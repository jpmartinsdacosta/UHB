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
#define UHB_ACL_CONFIG_CURRENT  "../config/current/acl.sh"

FlagCollection get_acl_fc, set_acl_fc;

const char *non_acl_fs_types[] = {
    "vfat",
    "ntfs",
    "iso9660",
    "squashfs",
    "tmpfs",
    "udf",
    NULL
};

const char get_acl_flags[] = {
    'a',    // Display the file access control list
    'd',    // Display the default access control list
    'c',    // Do not displau the comment header
    'e',    // Print all effective rights comments, even if identical to the rights defined by the ACL entry.
    'E',    // Do not print effective rights comments.
    's',    // Skip files that only have the base ACL entries (owner, group, others).
    'R',    // List the ACLs of all files and directories recursively.
    'L',    // Logical walk, follow symbolic links to directories. The default behavior is to follow symbolic link arguments, and skip symbolic links encountered in subdirectories. Only effective in combination with -R.
    'P',    // Physical walk, do not follow symbolic links to directories. This also skips symbolic link arguments. Only effective in combination with -R.
    't',    // Use an alternative tabular output format. The ACL and the default ACL are displayed side by side. Permissions that are ineffective due to the ACL mask entry are displayed capitalized. The entry tag names for the ACL_USER_OBJ and ACL_GROUP_OBJ entries are also displayed in capital letters, which helps in spotting those entries.
    'p',    // Do not strip leading slash characters (`/'). The default behavior is to strip leading slash characters.
    'n',    // List numeric user and group IDs
    'v'
};

const char set_acl_flags[] = {
    'b',    // Remove all extended ACL entries. The base ACL entries of the owner, group and others are retained.
    'k',    // Remove the Default ACL. If no Default ACL exists, no warnings are issued.
    'n',    // Do not recalculate the effective rights mask. The default behavior of setfacl is to recalculate the ACL mask entry, unless a mask entry was explicitly given.
    'd',    // All operations apply to the Default ACL.
    'R',    // Apply operations to all files and directories recursively.
    'L',    // Logical walk, follow symbolic links to directories. The default behavior is to follow symbolic link arguments, and skip symbolic links encountered in subdirectories.
    'P'     // Physical walk, do not follow symbolic links to directories.
};

bool acl_exists() {
    if(exec_exists("getfacl")){
        return true;
    }else{
        printf("MSG: ACL was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

int acl_incompatible_fs(char *filepath) {
    char command[MAX_LINE_LENGTH];
    snprintf(command, sizeof(command),"df -T %s 2>/dev/null | awk 'NR==2 {print $2}'", filepath);
    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("popen");
        return -1;
    }
    char fs_type[64] = {0};
    if (fgets(fs_type, sizeof(fs_type), fp) == NULL) {
        pclose(fp);
        fprintf(stderr, "ERR: Could not determine filesystem type.\n");
        return -1;
    }
    pclose(fp);
    fs_type[strcspn(fs_type, "\n")] = 0;
    for (int i = 0; non_acl_fs_types[i]; i++) {
        if (strcmp(fs_type, non_acl_fs_types[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

bool get_acl() {
    char flags[MAX_LINE_LENGTH];
    char path[MAX_FILEPATH_SIZE];
    char command[MAX_LINE_LENGTH];
    init_flag(&get_acl_fc,13,get_acl_flags);
    if(get_filepath(path)){
        get_user_input("MSG: Please enter additional flags followed by a single '-':", flags, MAX_LINE_LENGTH);
        if(check_flags(flags,&get_acl_fc) && acl_incompatible_fs(path) == 0){
            printf("MSG: Press q to exit current view.\n");
            snprintf(command, sizeof(command), "getfacl \"%s\" \"%s\" | less", flags, path);
            system(command);
            return true;
        }
        return false;
    }else{
        fprintf(stderr, "ERR: get_acl(): ACL could not be retrieved.\n");
        return false;
    }
}

bool set_acl() {
    char flags[MAX_LINE_LENGTH];
    char path[MAX_FILEPATH_SIZE];
    char acl_spec[MAX_FILEPATH_SIZE];
    char command[MAX_LINE_LENGTH];
    init_flag(&set_acl_fc,13,set_acl_flags);
    if(get_filepath(path)){
        get_user_input("MSG 1/2: Please enter ACL flags to be used, followed by a single '-':",flags,MAX_LINE_LENGTH);
        get_user_input("MSG 2/2: Please enter the ACL specification:",acl_spec,MAX_LINE_LENGTH);
        printf("MSG: Setting ACL...\n");
        if(check_flags(flags,&get_acl_fc) && acl_incompatible_fs(path) == 0){
            add_acl_element(path,command);
            snprintf(command,sizeof(command), "setfacl %s %s %s",flags, acl_spec, path);
            append_to_file(command, UHB_ACL_CONFIG_CURRENT);
            return true;
        }else{
            fprintf(stderr, "ERR: set_acl(): ACL could not be set.\n");
            return false;
        }
    }else{
        printf("ERR: Invalid/non-existent path.\n");
        return false;
    }
}

void view_acl_configuration() {
    view_file(UHB_ACL_CONFIG_CURRENT);
}

void reset_acl_configuration() {
    copy_file(UHB_SCRIPT_TEMPLATE_PATH,UHB_ACL_CONFIG_CURRENT);
}

void apply_acl_configuration() {
    char command[MAX_LINE_LENGTH];
    snprintf(command,sizeof(command),"sh %s",UHB_ACL_CONFIG_CURRENT);
    if(system(command) == 0){
        printf("MSG: ACL configuration applied successfully.\n");
    }else{
        fprintf(stderr,"ERR: Unable to apply ACL configuration.\n");
    }
}