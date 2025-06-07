#include <stdio.h>
#include <stdbool.h>

#include "acl.h"
#include "file.h"
#include "config.h"
#include "utils.h"
#include "input_output.h"
#include "module_var.h"
#include "global_var.h"
#include "policy.h"

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Filepath to the configuration files to be used/edited in UHB.
#define UHB_ACL_CONFIG_CURRENT  "../config/current/acl.sh"

FlagCollection get_acl_fc, set_acl_fc;

static const char *non_acl_fs_types[] = {
    "vfat",
    "ntfs",
    "iso9660",
    "squashfs",
    "tmpfs",
    "udf",
    NULL
};

static const char get_acl_flags[] = {
    'd',    // The operation applies to	the default ACL	of a directory instead of the access ACL.
    'h',    // If the target of the operation is a symbolic link, return the ACL from	the symbolic link itself  rather  than	following  the link.
    'i',    // For  NFSv4  ACLs,  append numerical ID at the end of each entry containing user or group	name.  Ignored for POSIX.1e ACLs.
    'n',    // Display user and	group IDs numerically rather  than  converting to a user or group name.	 Ignored for POSIX.1e ACLs.
    'q',    // Do  not	write commented	information about file name and	ownership.  This is useful when dealing with filenames with unprintable characters.
    'v',     // For NFSv4 ACLs, display access mask  and	 flags	in  a  verbose form.  Ignored for POSIX.1e ACLs.
    NULL
};

const char set_acl_flags[] = {
    'b',    // Remove all extended ACL entries. The base ACL entries of the owner, group and others are retained.
    'k',    // Remove the Default ACL. If no Default ACL exists, no warnings are issued.
    'n',    // Do not recalculate the effective rights mask. The default behavior of setfacl is to recalculate the ACL mask entry, unless a mask entry was explicitly given.
    'd',    // All operations apply to the Default ACL.
    'R',    // Apply operations to all files and directories recursively.
    'L',    // Logical walk, follow symbolic links to directories. The default behavior is to follow symbolic link arguments, and skip symbolic links encountered in subdirectories.
    'P',     // Physical walk, do not follow symbolic links to directories.
    NULL
};

bool acl_exists() {
    if(exec_exists("getfacl")){
        return true;
    }else{
        printf("MSG: ACL was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

bool get_acl() {
    char flags[MAX_LINE_LENGTH];
    char path[MAX_FILEPATH_SIZE];
    char command[MAX_LINE_LENGTH];
    init_flag(&get_acl_fc, sizeof(get_acl_flags)-1, get_acl_flags);
    if(get_filepath(path)){
        get_user_input("MSG: Please enter additional flags followed by a single '-':", flags, MAX_LINE_LENGTH);
        if(check_flags(flags,&get_acl_fc)){
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
    char input[MAX_LINE_LENGTH];
    char acl_flag[8];
    char acl_entries[512];
    char filepath[MAX_LINE_LENGTH];
    int opt = 1;
    while (opt == 1) {
        get_user_input("MSG: Please enter ACL entry to be added:", input, sizeof(input));
        parse_setfacl_command(input, acl_flag, sizeof(acl_flag),acl_entries, sizeof(acl_entries),filepath, sizeof(filepath));
        printf("ACL Flag: %s\n", acl_flag);
        printf("ACL Entries: %s\n", acl_entries);
        printf("Target Path: %s\n", filepath);
        opt = three_option_input("MSG: Is the provided information correct? (Y)es/(N)o/E(x)it:", 'Y', 'N', 'X');
    }
    if (opt == 0) {
        if(path_exists(filepath)){
            printf("MSG: Setting ACL...\n");
            append_to_file(input, UHB_ACL_CONFIG_CURRENT);
            add_acl_element(acl_flag,acl_entries,filepath);
        }else{
            fprintf(stderr, "MSG: Unable to add ACL entry.\n");
        }
    }
}

void rem_acl_rule() {
    remove_last_n_lines(UHB_ACL_CONFIG_CURRENT,1);
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