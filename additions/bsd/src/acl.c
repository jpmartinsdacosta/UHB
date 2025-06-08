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
static bool set_acl_fc_initialized = false;

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
    init_flag(&get_acl_fc,13,get_acl_flags);
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

void parse_acl_entry(const char *entry, char *typeBuf, size_t typeSize,
                     char *nameBuf, size_t nameSize,
                     char *permBuf, size_t permSize) {
    char temp[512];
    strncpy(temp, entry, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char *token = strtok(temp, ":");
    if (token) strncpy(typeBuf, token, typeSize - 1);
    typeBuf[typeSize - 1] = '\0';

    token = strtok(NULL, ":");
    if (token) strncpy(nameBuf, token, nameSize - 1);
    nameBuf[nameSize - 1] = '\0';

    token = strtok(NULL, ":");
    if (token) strncpy(permBuf, token, permSize - 1);
    permBuf[permSize - 1] = '\0';
}

// Extract ACL entries following -m, -x, etc.
int extract_acl_entries(const char *input, const char *flag, char *aclBuf, size_t aclBufSize) {
    char inputCopy[MAX_LINE_LENGTH];
    strncpy(inputCopy, input, sizeof(inputCopy));
    inputCopy[sizeof(inputCopy) - 1] = '\0';

    char *token = strtok(inputCopy, " ");
    while (token) {
        if (strcmp(token, flag) == 0) {
            token = strtok(NULL, " ");
            if (token) {
                strncpy(aclBuf, token, aclBufSize - 1);
                aclBuf[aclBufSize - 1] = '\0';
                return 1;
            }
        }
        token = strtok(NULL, " ");
    }
    aclBuf[0] = '\0';
    return 0;
}

// Find the last token (commonly used for the target path)
int extract_target_path(const char *input, char *targetBuf, size_t targetBufSize) {
    char inputCopy[MAX_LINE_LENGTH];
    strncpy(inputCopy, input, sizeof(inputCopy));
    inputCopy[sizeof(inputCopy) - 1] = '\0';

    char *token = strtok(inputCopy, " ");
    char *lastToken = NULL;
    while (token) {
        lastToken = token;
        token = strtok(NULL, " ");
    }

    if (lastToken) {
        strncpy(targetBuf, lastToken, targetBufSize - 1);
        targetBuf[targetBufSize - 1] = '\0';
        return 1;
    }

    targetBuf[0] = '\0';
    return 0;
}

// Main parser function
bool parse_setfacl_command(char *input, char *flagBuf, size_t flagBufSize,
                            char *entryBuf, size_t entryBufSize,
                            char *pathBuf, size_t pathBufSize,
                            FlagCollection *fc)
{
    char *tokens[256];
    int tokenCount = 0;

    char *token = strtok(input, " \t\n");
    while (token && tokenCount < 256) {
        tokens[tokenCount++] = token;
        token = strtok(NULL, " \t\n");
    }

    if (tokenCount < 2) {
        fprintf(stderr, "ERR: parse_setfacl_command(): Not enough tokens.\n");
        return false;
    }

    // Extract flags (e.g., from "setfacl -d")
    if (tokens[1][0] == '-') {
        strncpy(flagBuf, tokens[1], flagBufSize - 1);
        flagBuf[flagBufSize - 1] = '\0';

        if (!check_flags(flagBuf, fc)) {
            fprintf(stderr, "ERR: Invalid or duplicate flags.\n");
            return false;
        }
    } else {
        fprintf(stderr, "ERR: Expected flag token after command.\n");
        return false;
    }

    // Extract entry: first non-flag token after flag
    for (int i = 2; i < tokenCount - 1; i++) {
        if (tokens[i][0] != '-') {
            strncpy(entryBuf, tokens[i], entryBufSize - 1);
            entryBuf[entryBufSize - 1] = '\0';
            break;
        }
    }

    // Extract path: last token
    strncpy(pathBuf, tokens[tokenCount - 1], pathBufSize - 1);
    pathBuf[pathBufSize - 1] = '\0';

    return true;
}

bool set_acl() {
    char input[MAX_LINE_LENGTH];
    char input_copy[MAX_LINE_LENGTH];  // Copy for parsing
    char acl_flag[8];
    char acl_entries[512];
    char filepath[MAX_LINE_LENGTH];
    if (!set_acl_fc_initialized) {
        init_flag(&set_acl_fc, strlen(set_acl_flags), set_acl_flags);
        set_acl_fc_initialized = true;
    }
    int opt = 1;
    while (opt == 1) {
        get_user_input("MSG: Please enter ACL entry to be added:", input, sizeof(input));

        // Make a copy for parsing so original stays intact
        strncpy(input_copy, input, sizeof(input_copy));
        input_copy[sizeof(input_copy) - 1] = '\0';

        if (!parse_setfacl_command(input_copy, acl_flag, sizeof(acl_flag),
                                   acl_entries, sizeof(acl_entries),
                                   filepath, sizeof(filepath), &set_acl_fc)) {
            fprintf(stderr, "ERR: Failed to parse ACL input.\n");
            continue;
        }
        printf("ACL Flag: %s\n", acl_flag);
        printf("ACL Entries: %s\n", acl_entries);
        printf("Target Path: %s\n", filepath);
        opt = three_option_input("MSG: Is the provided information correct? (Y)es/(N)o/E(x)it:", 'Y', 'N', 'X');
    }
    if (opt == 0) {
        if (path_exists(filepath)) {
            printf("MSG: Setting ACL...\n");
            append_to_file(input, UHB_ACL_CONFIG_CURRENT);  // Append original, unmodified input
            add_acl_element(acl_flag, acl_entries, filepath,is_recursive(acl_flag));
            return true;
        } else {
            fprintf(stderr, "MSG: Unable to add ACL entry. File does not exist.\n");
        }
    }
    return false;
}

void rem_acl_rule() {
    remove_last_n_lines(UHB_ACL_CONFIG_CURRENT,1);
    rem_acl_element();
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

void free_acl() {
    free_fc(&set_acl_fc);
    free_fc(&get_acl_fc);
    set_acl_fc_initialized = false;
}