#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__linux__)
// Debian headers.
#endif

#define MAX_CMD 300             // Maximum length of a command.

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
