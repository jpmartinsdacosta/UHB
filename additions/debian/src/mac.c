#include <stdio.h>
#include <stdbool.h>

bool mac_exists() {
    printf("MSG: MAC was NOT detected. Configuration will NOT be applied.\n");
    return false; 
}

/**
 * NO MAC TO BE ADDED FOR DEBIAN
 */