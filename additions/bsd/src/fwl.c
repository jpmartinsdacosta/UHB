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
#define BSD_FIREWALL_RULES_ORIGINAL "/etc/ipfw.rules"

// Filepath to the configuration files to be used/edited in UHB.
#define BSD_FIREWALL_RULES_CURRENT  "/root/uhb/base/config/current/ipfw.rules"

// Filepath to the backup of all configuration files.
#define BSD_FIREWALL_RULES_BACKUP   "/root/uhb/base/config/backups/ipfw.rules"

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

void initialize_firewall(bool copy_from_backup) {
    if(!copy_from_backup){
        copy_file(BSD_FIREWALL_RULES_ORIGINAL,BSD_FIREWALL_RULES_CURRENT);
        copy_file(BSD_FIREWALL_RULES_ORIGINAL,BSD_FIREWALL_RULES_BACKUP);
    }else{
        copy_file(BSD_FIREWALL_RULES_BACKUP,BSD_FIREWALL_RULES_CURRENT);
    }
}

void add_firewall_rule() {
    char rule[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG: Please insert the firewall rule to be added:",rule,sizeof(rule));
        opt = three_option_input("Is the information correct? (Y)es/(N)o/E(x)it",'Y','N','X');
    }
    if(opt == 0){
        append_to_file(rule,BSD_FIREWALL_RULES_CURRENT);
    }
}

void view_firewall_configuration() {
    view_file(BSD_FIREWALL_RULES_CURRENT);
}

void reset_firewall_configuration() {
    copy_file(BSD_FIREWALL_RULES_ORIGINAL, BSD_FIREWALL_RULES_CURRENT);
}

void apply_firewall_configuration() {
    copy_file(BSD_FIREWALL_RULES_CURRENT, BSD_FIREWALL_RULES_ORIGINAL);
}

void view_firewall_manual(){
    system("man ipfw");
    system("clear");
}