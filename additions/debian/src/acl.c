#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool acl_exists() {
    if(system("command -v getfacl > /dev/null 2>&1") == 0){
        printf("MSG: ACL was detected. Configuration will be applied.\n");
        return true;
    }else{
        printf("MSG: ACL was NOT detected. Configuration will not be applied.\n");
        return false;
    }
}