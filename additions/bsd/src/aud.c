#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <string.h>

#include "global_var.h"
#include "module_var.h"
#include "file.h"
#include "utils.h"
#include "input_output.h"
#include "aud.h"

#define AUDIT_CONTROL "/etc/security/audit_control"
#define AUDIT_USER "/etc/security/audit_user"
#define AUDIT_WARN "/etc/security/audit_warn"

#define UHB_AUDIT_CONTROL "/root/uhb/base/config/templates/audit_control"
#define UHB_AUDIT_USER "/root/uhb/base/config/templates/audit_user"
#define UHB_AUDIT_WARN "/root/uhb/base/config/templates/audit_warn"

const char *audit_control_options[] = {
    "dir",
    "dist",
    "flags",
    "host",
    "naflags",
    "minfree",
    "policy",
    "filesz",
    "expire-after",
    "qsize",
    NULL
};

const char *audit_control_params[] = {
    "cnt",
    "ahlt",
    "argv",
    "arge",
    "seq",
    "group",
    "trail",
    "path",
    "zonename",
    "perzone",
    NULL
};

bool aud_exists() {
    if(exec_exists("auditd")){
        return true;
    }else{
        printf("MSG: Audit daemon was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

bool check_auditing_status() {
    char command[MAX_LINE_LENGTH];
    snprintf(command, sizeof(command), "service auditd status >/dev/null 2>&1");
    if(system(command) == 0){
        printf("MSG: Auditing daemon is running.\n");
        return true;
    }else{
        printf("MSG: Auditing daemon is NOT running.\n");
        return false;
    }
}

bool restart_auditing_daemon() {
    char command[MAX_LINE_LENGTH];
    printf("MSG: Restarting auditing daemon...\n");
    if(enabled_in_rc_conf("auditd_enable")){
        snprintf(command, sizeof(command), "service auditd restart >/dev/null 2>&1");
        if(system(command) == 0){
            printf("MSG: Auditing daemon successfully restarted.\n");
            return true;
        }else{
            printf("WRN: Unable to restart auditing daemon.\n");
            return false;
        }
    }else{
        printf("ERR: Auditing daemon must be mannualy enabled in rc.conf.\n");
        return false;
    }
    
}

void check_auditing_config(){
    int opt = three_option_input("MSG 1/3: Show audit_control file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    switch (opt){
        case 0:
            view_file(UHB_AUDIT_CONTROL);
            break;
        case 1:
            break;
        case 2:
            return;
            break;
    }
    opt = three_option_input("MSG 2/3: Show audit_user file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    switch (opt){
        case 0:
            view_file(UHB_AUDIT_USER);
            break;
        case 1:
            break;
        case 2:
            return;
            break;
    }
    opt = three_option_input("MSG 3/3: Show audit_warn file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    switch (opt){
        case 0:
            view_file(UHB_AUDIT_WARN);
            break;
        case 1:
            break;
        case 2:
            return;
            break;
    }
}

void add_audit_control_option() {
    char param[MAX_LINE_LENGTH];
    bool leave = false;
    int opt;
    for(int i = 0; i < get_string_array_size(audit_control_options) && !leave; i++){
        printf("MSG %d/10: Modify the %s option? ",i,audit_control_options[i]);
        opt = three_option_input("(Y)es/(N)o/E(x)it:",'Y','N','X');
        if(opt == 0){
            get_user_input("MSG: Insert new parameter:",param,sizeof(param));     
            replace_option_value(audit_control_options,':',param,UHB_AUDIT_CONTROL);
        }
        if(opt == 2){
            leave = true;
        }

    }
}

void add_audit_user_option() {
    char param[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG: Add the new parameter for audit_user:",param,sizeof(param));
        opt = three_option_input("MSG: Is the parameter correct? (Y)es/(N)o/E(x)it:",'Y','N','X');
    }
    if(opt == 0){
        append_to_file(param,UHB_AUDIT_USER);
    }
}

void add_audit_warn_option() {
    char param[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG: Add the new parameter for audit_warn:",param,sizeof(param));
        opt = three_option_input("MSG: Is the parameter correct? (Y)es/(N)o/E(x)it:",'Y','N','X');
    }
    if(opt == 0){
        append_to_file(param,UHB_AUDIT_WARN);
    }
}

// Remote auditing config is not to be added in FreeBSD.