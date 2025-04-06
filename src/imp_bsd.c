#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__FreeBSD__)
// FreeBSD headers.
#endif

#define MAX_FILE_PATH 200           // Maximum length of a file path.
#define MAX_CMD 300                 // Maximum length of a command.

void exec_exists_bsd (bool exec[4]){
    const char *programs[4] = {"getfacl", "ipfw", "rsyslogd","auditd"};
    char command[MAX_CMD];
    for(int i = 0; i < 4; i++){
        snprintf(command, sizeof(command), "command -v \"%s\" >/dev/null 2>&1", programs[i]);
        if(system(command) == 0){
            printf("INI: %s exists.\n", programs[i]);
            exec[i] = true;
        }else{
            printf("INI: %s does NOT exist.\n", programs[i]);
            exec[i] = false;
        }
    }
}

bool is_acl_enabled_bsd(char *filepath){
    char* filesystem;
    filesystem = find_partition_from_file(filepath);
    return find_strings_in_line(filesystem,"acls","/etc/fstab");
}

bool set_acl(){
    char path[MAX_FILE_PATH];
    char options[MAX_CMD];
    char command[MAX_CMD];
    get_filepath(path);
    if(!is_acl_enabled_bsd(path)){
        printf("ERR: ACLs are not enabled in the filesystem of the given file.\n");
        printf("ERR: Make sure the enable them in your BSD system visa fstab.\n");
    }else{
        if(path_exists(path) && !acl_incompatible_fs(path)){
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
}


void test_function(){
    printf("BSD: Test function executed.\n");
}
