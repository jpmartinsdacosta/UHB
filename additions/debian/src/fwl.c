#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"
#include "utils.h"

bool fwl_exists() {
    if(exec_exists("ufw")){
        return true;
    }else{
        printf("MSG: Firewall was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}