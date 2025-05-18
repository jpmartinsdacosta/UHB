#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool mac_exists() {
    /*
    Change the command to detect in FreeBSD
    if(system("command -v auditd > /dev/null 2>&1") == 0){
        printf("INI: Audit daemon was detected.\n");
        return true;
    }else{
        printf("INI: Audit daemon was NOT detected.\n");
        return false;
    }
    */
   return false;
}