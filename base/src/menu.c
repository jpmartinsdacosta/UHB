#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "module_var.h"
#include "acl.h"
#include "mac.h"
#include "log.h"
#include "aud.h"
#include "fwl.h"
#include "utils.h"
#include "file.h"
#include "config.h"
#include "policy.h"
#include "input_output.h"
#include "global_var.h"
#include "os_interface.h"

const char *main_menu_options[] = {
    "1. Get/Set Discretionary Access Control on a file/directory",
    "2. Get/Set Access Control List(s) on a file/directory",
    "3. MAC configuration",
    "4. Logging configuration",
    "5. Auditing configuration",
    "6. Firewall configuration",
    "7. Configuration file options",
    "8. ...",
    "9. Test specific function",
    "0. Exit UHB",
    NULL
};

const char *dac_menu_options[] = {
    "1. Get DAC of a file/directory",
    "2. Set DAC of a file/directory",
    "3. View current DAC configuration",
    "4. View ls manual page",
    "5. View chmod manual page",
    "6. View chown manual page",
    "0. Return to Main Menu",
    NULL
};

const char *acl_menu_options[] = {
    "1. Get ACL of a file/directory",
    "2. Set ACL of a file/directory",
    "3. View current ACL configuration",
    "4. View getfacl manual page",
    "5. View setfacl manual page",
    "0. Return to Main Menu",
    NULL
};

const char *mac_menu_options[] = {
    "1. Get MAC of a file/directory",
    "2. Set MAC of a file/directory",
    "3. View current MAC configuration",
    "0. Return to Main Menu",
    NULL
};

const char *log_menu_options[] = {
    "1. Change RFC logging standard",
    "2. Add a local logging rule",
    "3. Set up a logging server",
    "4. Add a location where to store remote logs",
    "5. Add logs to be forwarded to a remote server",
    "6. View current logging configuration",
    "7. Reset current logging configuration",
    "0. Return to Main Menu",
    NULL
};

const char *aud_menu_options[] = {
    "1. Configure auditing in the system",
    "2. Configure auditing reception service",
    "3. Configure auditing forwarding service",
    "4. View current auditing configuration",
    "5. Reset current auditing configuration",
    "0. Return to Main Menu",
    NULL
};

const char *fwl_menu_options[] = {
    "1. Check for open ports",
    "2. Add a firewall rule",
    "3. View current firewall configuration",
    "4. Reset current firewall configuration",
    "0. Return to Main Menu",
    NULL
};

const char *conf_menu_options[] = {
    "1. Load configuration from backup",
    "2. View configuration file",
    "3. Clear configuration file",
    "4. Apply changes from configuration file",
    "5. Export current configuration file",
    "0. Return to Main Menu",
    NULL
};

void clear_conf_prompt(){
    int choice = -1;
    while(choice == -1){
        choice = get_yes_no_input("MSG: Are you sure that you want to clear the config file? (y/n):\n");
        if(choice == 0){
            reset_conf();
            clear_dac_array();
            init_dac_array();
            printf("MSG: Config file cleared...\n");
        }else{
            printf("MSG: Exiting UHB...\n");
        }
    }
}

void final_prompt(){
    int choice = -1;
    while(choice == -1 && is_conf_file_mod()){
        choice = get_yes_no_input("MSG: Clear the config file before leaving? (y/n):\n");
        if(choice == 0){
            reset_conf();
            clear_dac_array();
            printf("MSG: Config file cleared, exiting UHB...\n");
        }else{
            printf("MSG: Exiting UHB...\n");
        }
    }
}

void dac_menu(){
    int choice = -1;
    while(choice != 0){
        choice = select_string_array("DAC Menu", dac_menu_options);
        switch(choice){
            case 1:
                get_dac();
                break;
            case 2:
                set_dac();
                break;
            case 3:
                view_file(CONFIG_DAC);
                break;
            case 4:
                system("man ls");
                break;
            case 5:
                system("man chmod");
                break;
            case 6:
                system("man chown");
                break;
            case 0:
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}

void acl_menu(){
    int choice = -1;
    while(choice != 0){
        choice = select_string_array("ACL Menu", acl_menu_options);
        switch(choice){
            case 1:
                //get_acl();
                break;
            case 2:
                //set_acl();
                break;
            case 3:
                view_file(CONFIG_ACL);
                break;
            case 4:
                system("man getfacl");
                break;
            case 5:
                system("man setfacl");
                break;
            case 0:
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}

void mac_menu(){
    int choice = -1;
    while(choice != 0){
        choice = select_string_array("MAC Menu", mac_menu_options);
        switch(choice){
            case 1:
                printf("MSG: Option not implemented yet.\n");
                break;
            case 2:
                printf("MSG: Option not implemented yet.\n");
                break;
            case 3:
                view_file(CONFIG_MAC);
                break;
            case 0:
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}

void log_menu(){
    int choice = -1;
    while(choice != 0){
        choice = select_string_array("Logging daemon Menu", log_menu_options);
        switch(choice){
            case 1:
                apply_rfc5424();
                break;
            case 2:
                add_local_logging();
                break;
            case 3:
                set_log_reception_service();
                break;
            case 4:
                add_log_reception_rule();
                break;
            case 5:
                add_log_forwarding_rule();
                break;
            case 6:
                view_file(CONFIG_LOG);
                break;
            case 7:
                break;
            case 0:
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}

void aud_menu(){
    int choice = -1;
    while(choice != 0){
        choice = select_string_array("Auditing daemon Menu", aud_menu_options);
        switch(choice){
            case 1:
                add_local_auditing();
                break;
            case 2:
                configure_auditing_reception_service();
                break;
            case 3:
                configure_auditing_forwarding_service();
                break;
            case 4:
                reset_auditing_configuration();
                break;
            case 5:
                view_auditing_configuration();
                break;
            case 0:
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}

void fwl_menu(){
    int choice = -1;
    while(choice != 0){
        choice = select_string_array("Firewall Menu", fwl_menu_options);
        switch(choice){
            case 1:
                printf("MSG: Option not implemented yet.\n");
                break;
            case 2:
                printf("MSG: Option not implemented yet.\n");
                break;
            case 3:
                view_file(CONFIG_FWL);
                break;
            case 0:
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}

void conf_menu(){
    int choice = -1;
    while(choice != 0){
        choice = select_string_array("Configuration File Menu", conf_menu_options);
        switch(choice){
            case 1:
                // Load configuration from backup 
                printf("MSG: Option not implemented yet.\n");
                break;
            case 2:
                view_file(CONFIG_UHB);
                break;
            case 3:
                clear_conf_prompt();
                break;
            case 4:
                apply_service_conf();
                break;
            case 5:
                printf("MSG: Option not implemented yet.\n");
                break;
            case 0:
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}

void main_menu(){
    int choice = -1;
    while(choice != 0){
        choice = select_string_array("UHB Main Menu", main_menu_options);
        switch(choice){
            case 1:
                dac_menu();
                break;
            case 2:
                acl_exists() ? acl_menu() : printf("MSG: Option not available.\n");
                break;
            case 3:
                mac_exists() ? mac_menu() : printf("MSG: Option not available.\n");
                break;
            case 4:
                log_exists() ? log_menu() : printf("MSG: Option not available.\n");
                break;
            case 5:
                aud_exists() ? aud_menu() : printf("MSG: Option not available.\n");
                break;
            case 6:
                fwl_exists() ? fwl_menu() : printf("MSG: Option not available.\n");
                break;
            case 7:
                conf_menu();
                break;
            case 8:
                printf("MSG: Option not implemented yet.\n");
                break;
            case 9:
                replace_string_in_line("/root/uhb/base/config/templates/auditdistd.conf",20,"\"<addr>\"","\"PECO\"");
                break;
            case 0:
                reset_conf();
                printf("\nFIN: Goodbye!\n");
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}
