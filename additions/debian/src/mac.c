#include <stdio.h>
#include <stdbool.h>

bool mac_exists() {
    printf("MSG: MAC was NOT detected. Configuration will NOT be applied.\n");
    return false; 
}

/**
 * NO MAC TO BE ADDED FOR DEBIAN, EMPTY FUNCTIONS BELOW.
 */

bool get_mac(){
    return false;
}

int parse_mac_mode_flags(const char *mode_str) {
    return 0;
}

bool set_mac(){
    return false;
}

void view_mac_manual(){

}

void view_mac_configuration(){

}

void reset_mac_configuration(bool load_from_backup){

}

void apply_mac_configuration(){

}

void free_mac(){

}
