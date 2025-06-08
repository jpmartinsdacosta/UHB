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
    if(!copy_from_backup){
        copy_file(DEB_FIREWALL_CONFIG_ORIGINAL,DEB_FIREWALL_CONFIG_CURRENT);
        copy_file(DEB_FIREWALL_CONFIG_ORIGINAL,DEB_FIREWALL_CONFIG_BACKUP);
        copy_file(DEB_FIREWALL_USERRULES_ORIGINAL,DEB_FIREWALL_USERRULES_CURRENT);
        copy_file(DEB_FIREWALL_USERRULES_ORIGINAL,DEB_FIREWALL_USERRULES_BACKUP);
    }else{
        copy_file(DEB_FIREWALL_CONFIG_BACKUP,DEB_FIREWALL_CONFIG_CURRENT);
        copy_file(DEB_FIREWALL_USERRULES_BACKUP,DEB_FIREWALL_USERRULES_CURRENT);
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
        char test_cmd[MAX_LINE_LENGTH + 32];
        snprintf(test_cmd, sizeof(test_cmd), "ufw --dry-run %s > /dev/null 2>&1", rule);
        if (system(test_cmd) == 0) {
            append_to_file(rule, DEB_FIREWALL_USERRULES_CURRENT);
        } else {
            fprintf(stderr, "ERR: Invalid firewall rule syntax.\n");
        }
    }
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

void apply_firewall_configuration() {
    copy_file(DEB_FIREWALL_CONFIG_CURRENT, DEB_FIREWALL_CONFIG_ORIGINAL);
    copy_file(DEB_FIREWALL_USERRULES_CURRENT, DEB_FIREWALL_USERRULES_ORIGINAL);
    system("ufw reload");
}

void view_firewall_manual(){
    system("man ufw");
    system("clear");
}

const char *fwl_logging_options[] = {
    "1. Off: Disable logging",
    "2. Low: Log only blocked packets (default)",
    "3. Medium: Logs blocked + limited allowed packets",
    "4. High: Logs most packets (no rate limiting)",
    "5. Full: Logs everything (can be verbose)",
    "0. Cancel and exit",
    NULL
};

void configure_firewall_logging() {
    int choice = -1;
    while(choice != 0){
        choice = select_string_array("UFW logging options", fwl_logging_options);
        switch(choice){
            case 1:
                system("ufw logging off");
                break;
            case 2:
                system("ufw logging low");
                break;
            case 3:
                system("ufw logging medium");
                break;
            case 4:
                system("ufw logging high");
                break;
            case 5:
                system("ufw logging full");
                break;
            case 0:
                break;
            
        }
    }
}