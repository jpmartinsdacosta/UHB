#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

bool mac_exists() {
    printf("MSG: MAC was NOT detected. Configuration will NOT be applied.\n");
    return false; 
}

// Not implemented in Debian at the moment.