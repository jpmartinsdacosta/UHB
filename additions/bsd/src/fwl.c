#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool fwl_exists() {
    if(system("command -v ipfw > /dev/null 2>&1") == 0){
        printf("MSG: Firewall was detected. Configuration will be applied.\n");
        return true;
    }else{
        printf("MSG: Firewall was NOT detected. Configuration will not be applied.\n");
        return false;
    }
}