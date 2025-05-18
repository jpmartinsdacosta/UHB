#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool log_exists() {
    if(system("command -v rsyslog > /dev/null 2>&1") == 0){
        printf("MSG: Logging daemon was detected. Configuration will be applied.\n");
        return true;
    }else{
        printf("MSG: Logging daemon was NOT detected. No configuration will be applied.\n");
        return false;
    }
}