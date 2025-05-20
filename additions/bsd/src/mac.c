#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool mac_exists() {
    /*
    Change the command to detect in FreeBSD
    if(exec_exists("mac_bsdextended")){
        return true;
    }else{
        printf("INI: Audit daemon was NOT detected.\n");
        return false;
    }
    */
    printf("MSG: MAC was NOT detected. Configuration will NOT be applied.\n");
    return false;
}