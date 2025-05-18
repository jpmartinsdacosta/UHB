#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool aud_exists() {
    if(exec_exists("auditd")){
        printf("MSG: Audit daemon was detected.\n");
        return true;
    }else{
        printf("MSG: Audit daemon was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}