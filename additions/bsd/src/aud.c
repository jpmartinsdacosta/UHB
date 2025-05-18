#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool aud_exists() {
    if(system("command -v auditd > /dev/null 2>&1") == 0){
        printf("MSG: Audit daemon was detected. Configuration will be applied.\n");
        return true;
    }else{
        printf("MSG: Audit daemon was NOT detected. Configuration will not be applied.\n");
        return false;
    }
}