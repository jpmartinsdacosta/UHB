#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "imp_bsd.h"
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__FreeBSD__)
// FreeBSD headers.
#endif

int imp_bsd() {
    printf("BSD implementation\n");
    return 1;
}

void exec_exists_bsd (int option[4]){
    const char *programs[4] = {"getfacl", "ipfw", "rsyslogd","auditd"};
    for(int i = 0; i < 4; i++){
        char command[50];
        snprintf(command, sizeof(command), "which -s %s", programs[i]);
        if(system(command) == 0){
           option[i] = 1;
        }
    }
}