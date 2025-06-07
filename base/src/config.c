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

extern bool load_from_backup = false;
extern bool show_debug_messages = false;

void load_from_backup_function(){
    int opt = get_yes_no_input("MSG: Would you like to load from backup? (Y/N):");
    if(opt == 0){
        replace_option_value("load_from_backup",'=',"true",UHB_BASE_CONFIG_CURRENT);
        printf("MSG: Loading from backup enabled.\n");
    }else{
        replace_option_value("load_from_backup",'=',"false",UHB_BASE_CONFIG_CURRENT);
        printf("MSG: Loading from backup disabled.\n");
    }
}

void show_debug_messages_function(){
    int opt = get_yes_no_input("MSG: Show debug messages? (Y/N):");
    if(opt == 0){
        replace_option_value("show_debug_messages",'=',"true",UHB_BASE_CONFIG_CURRENT);
        printf("MSG: Showing debug messages.\n");
    }else{
        replace_option_value("show_debug_messages",'=',"false",UHB_BASE_CONFIG_CURRENT);
        printf("MSG: Hiding debug messages.\n");
    }
}

void reset_configuration_file() {
    if(get_yes_no_input("MSG: Reset UHB Base configuration file? (Y/N):") == 0)
        copy_file(UHB_BASE_CONFIG_BACKUP,UHB_BASE_CONFIG_CURRENT);
}

void view_configuration_file() {view_file(UHB_BASE_CONFIG_CURRENT);}

void reset_file_service_policy() {
    if(get_yes_no_input("MSG: Reset current file service policy? (Y/N):") == 0){
        printf("MSG: Resetting file service policy...\n");
        reset_dac_configuration();
        reset_acl_configuration();
        //reset_mac_configuration();
        clear_dac_array();
        init_dac_array();
    }
}

void apply_file_service_policy() {
    printf("MSG: Please double-check DAC, ACL and MAC configuration files!\n");
    if (get_yes_no_input("MSG: Apply DAC, ACL and MAC configuration to the system? (Y/N):") == 0){
        apply_dac_configuration();
        apply_acl_configuration();
    }
}

void initialize_uhb() {
    load_from_backup = find_string_in_file("load_from_backup = true",UHB_BASE_CONFIG_CURRENT);
    show_debug_messages = find_string_in_file("show_debug_messages = true",UHB_BASE_CONFIG_CURRENT);
    reset_dac_configuration();
    if(acl_exists())
        reset_acl_configuration();
    if(mac_exists())
        reset_mac_configuration();
    if(log_exists())
        initialize_logging(load_from_backup);  
    if(aud_exists()) 
        initialize_auditing(load_from_backup);
    if(fwl_exists())
        initialize_firewall(load_from_backup);
    init_all_arrays();
}

void terminate_uhb() {
    clear_all_arrays();
}