#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Filepath to the original configuration files.
#define BSD_FIREWALL_RULES_ORIGINAL "/etc/ipfw.rules"

// Filepath to the configuration files to be used/edited in UHB.
#define BSD_FIREWALL_RULES_CURRENT "/root/uhb/base/config/current/ipfw.rules"

// Filepath to the backup of all configuration files.
#define BSD_FIREWALL_RULES_BACKUP "/root/uhb/base/config/backups/ipfw.rules"

bool fwl_exists() {
    if(exec_exists("ipfw")){
        return true;
    }else{
        printf("MSG: Firewall was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

bool check_firewall_status() {
    if(system("service ipfw status >/dev/null 2>&1") == 0){
        return true;
    }else{
        return false;
    }
}

void view_firewall_configuration(){
    
}

void view_firewall_manual(){
    system("man ipfw");
    system("clear");
}