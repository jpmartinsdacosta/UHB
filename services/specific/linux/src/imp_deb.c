#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "menu.h"
#include "file.h"
#include "config.h"
#include "utils.h"
#include "io.h"
#include "os_interface.h"
#include "global_var.h"

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__linux__)
// Debian headers.
#endif

char os[] = "##uhb_os = DEB";

char* get_os(){
    return os;
}

void exec_exists (bool exec[4]){
    const char *programs[4] = {"getfacl", "ufw", "rsyslogd","auditd"};
    char command[MAX_LINE_LENGTH];
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
