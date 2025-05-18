#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"
#include "utils.h"

bool aud_exists() {
    if(exec_exists("auditd")){
        return true;
    }else{
        printf("MSG: Audit daemon was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}