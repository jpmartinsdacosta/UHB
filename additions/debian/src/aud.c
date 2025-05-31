#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <string.h>

#include "global_var.h"
#include "module_var.h"
#include "file.h"
#include "utils.h"
#include "input_output.h"
#include "aud.h"

bool aud_exists() {
    if(exec_exists("auditd")){
        return true;
    }else{
        printf("MSG: Audit daemon was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}