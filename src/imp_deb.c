#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "imp_deb.h"
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__linux__)
// Debian headers.
#endif

int imp_deb() {
    printf("DEB implementation\n");
    return 1;
}

void exec_exists_deb (int option[4]){
    const char *programs[4] = {"getfacl", "ufw", "rsyslogd","auditd"};
    for(int i = 0; i < 4; i++){
        char command[50];
        snprintf(command, sizeof(command), "command -v %s >/dev/null 2>&1", programs[i]);
        if(system(command) == 0){
           option[i] = 1;
        }
    }
}