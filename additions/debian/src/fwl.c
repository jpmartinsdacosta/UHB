#include <stdio.h>
#include <stdbool.h>

#include "file.h"
#include "config.h"
#include "utils.h"
#include "input_output.h"
#include "global_var.h"
#include "policy.h"
#include "fwl.h"

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Filepath to the original configuration files.
#define DEB_FIREWALL_CONFIG_ORIGINAL    "/etc/ufw/ufw.conf"
#define DEB_FIREWALL_USERRULES_ORIGINAL "/etc/ufw/user.rules"


// Filepath to the configuration files to be used/edited in UHB.
#define DEB_FIREWALL_CONFIG_CURRENT     "../config/current/ufw.conf"
#define DEB_FIREWALL_USERRULES_CURRENT  "../config/current/user.rules"

// Filepath to the backup of all configuration files.
#define DEB_FIREWALL_CONFIG_BACKUP      "../config/backups/ufw.conf"
#define DEB_FIREWALL_USERRULES_BACKUP   "../config/backups/user.rules"

bool fwl_exists() {
    if(exec_exists("ufw")){
        return true;
    }else{
        printf("MSG: Firewall was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

bool check_firewall_status() {
    if(system("service ufw status >/dev/null 2>&1") == 0){
        return true;
    }else{
        return false;
    }
}

void initialize_firewall(bool copy_from_backup) {
    
}

void add_firewall_rule() {
    
}

void view_firewall_configuration() {
    printf("MSG: ufw.conf:\n");
    view_file(DEB_FIREWALL_CONFIG_CURRENT);
    printf("MSG: user.rules:\n");
    view_file(DEB_FIREWALL_USERRULES_CURRENT);
}

void reset_firewall_configuration() {
    copy_file(DEB_FIREWALL_CONFIG_ORIGINAL, DEB_FIREWALL_CONFIG_CURRENT);
    copy_file(DEB_FIREWALL_USERRULES_ORIGINAL, DEB_FIREWALL_USERRULES_CURRENT);
}

void apply_firewall_confgiuration() {
    copy_file(DEB_FIREWALL_CONFIG_CURRENT, DEB_FIREWALL_CONFIG_ORIGINAL);
    copy_file(DEB_FIREWALL_USERRULES_CURRENT, DEB_FIREWALL_USERRULES_ORIGINAL);
}

void view_firewall_manual(){
    system("man ufw");
    system("clear");
}