#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

FlagCollection get_acl_fc, set_acl_fc;

const char get_acl_flags[] = {
    'd',    // The operation applies to	the default ACL	of a directory instead of the access ACL.
    'h',    // If the target of the operation is a symbolic link, return the ACL from	the symbolic link itself  rather  than	following  the link.
    'i',    // For  NFSv4  ACLs,  append numerical ID at the end of each entry containing user or group	name.  Ignored for POSIX.1e ACLs.
    'n',    // Display user and	group IDs numerically rather  than  converting to a user or group name.	 Ignored for POSIX.1e ACLs.
    'q',    // Do  not	write commented	information about file name and	ownership.  This is useful when dealing with filenames with unprintable characters.
    'v'     // For NFSv4 ACLs, display access mask  and	 flags	in  a  verbose form.  Ignored for POSIX.1e ACLs.
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