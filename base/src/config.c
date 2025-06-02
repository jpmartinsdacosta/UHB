#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dac.h"
#include "acl.h"
#include "mac.h"
#include "log.h"
#include "aud.h"
#include "fwl.h"
#include "input_output.h"
#include "file.h"
#include "config.h"
#include "policy.h"
#include "module_var.h"
#include "global_var.h"

void load_from_backup_function(){
    int opt = get_yes_no_input("MSG: Would you like to load from backup? (Y/N):");
    if(opt == 0){
        replace_option_value("load_from_backup",' ',"true",UHB_BASE_CONFIG_CURRENT);
        printf("MSG: Loading from backup enabled.\n");
    }else{
        replace_option_value("load_from_backup",' ',"false",UHB_BASE_CONFIG_CURRENT);
        printf("MSG: Loading from backup disabled.\n");
    }
}

void view_configuration_file() {view_file(UHB_BASE_CONFIG_CURRENT);}

void apply_file_service_policy() {
    printf("MSG: Please double-check DAC, ACL and MAC configuration files!\n");
    if (get_yes_no_input("MSG: Apply DAC, ACL and MAC configuration to the system? (Y/N):") == 0){
        apply_dac_configuration();
        apply_acl_configuration();
    }
}

void initialize_uhb() {
    init_dac_array();       // Initialize array of DACStruct
}

void terminate_uhb() {
    clear_dac_array();      // Free memory allocated to DACStruct
}