#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__linux__)
// Debian headers.
#endif

#define MAX_FILE_PATH 200           // Maximum length of a file path.
#define MAX_CMD 300                 // Maximum length of a command.

void exec_exists_deb (bool exec[4]){
    const char *programs[4] = {"getfacl", "ufw", "rsyslogd","auditd"};
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

bool set_acl(){
    char path[MAX_FILE_PATH];
    char options[MAX_CMD];
    char command[MAX_CMD];
    get_filepath(path);
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


void test_function(){
    printf("DEB: Test function executed.\n");
}
