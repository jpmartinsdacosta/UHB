#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool log_exists() {
    if(exec_exists("rsyslog")){
        printf("MSG: Logging daemon was detected.\n");
        return true;
    }else{
        printf("MSG: Logging daemon was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}