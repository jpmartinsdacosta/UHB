#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"
#include "utils.h"

bool acl_exists() {
    if(exec_exists("getfacl")){
        return true;
    }else{
        printf("MSG: ACL was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}