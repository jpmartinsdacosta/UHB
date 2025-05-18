#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool acl_exists() {
    if(exec_exists("getfacl")){
        printf("MSG: ACL was detected.\n");
        return true;
    }else{
        printf("MSG: ACL was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}