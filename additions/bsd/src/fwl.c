#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool fwl_exists() {
    if(exec_exists("ipfw")){
        return true;
    }else{
        printf("INI: Firewall was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}