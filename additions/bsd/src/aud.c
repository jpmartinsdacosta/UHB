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

// Original config files.
#define AUDIT_CONTROL "/etc/security/audit_control"
#define AUDIT_USER "/etc/security/audit_user"
#define AUDIT_WARN "/etc/security/audit_warn"

#define AUDITDISTD_CONF "/root/uhb/base/config/templates/auditdistd.conf"
#define AUDITDISTD_BACKUP "/root/uhb/base/config/backups/auditdistd.conf"

static bool remote_auditing_present = false;

/**
 * Functions regarding daemon detection, status and execution.
 */

bool aud_exists() {
    if(exec_exists("auditd")){
        return true;
    }else{
        return false;
    }
}

bool remote_auditing_daemon_exists(){
    if(exec_exists("auditdistd")){
        remote_auditing_present = true;
        return true;
    }else{
        remote_auditing_present = false;
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

/**
 * Functions regarding module configuration
 */

void initialize_auditing_module(bool copy_from_backup) {
    if(aud_exists() && check_auditing_status()){
        if(!copy_from_backup){
            // Copy original files to UHB
            copy_file(AUDIT_CONTROL,UHB_AUDIT_CONTROL); 
            copy_file(AUDIT_USER,UHB_AUDIT_USER);       
            copy_file(AUDIT_WARN,UHB_AUDIT_WARN); 
            if(remote_auditing_present)
                copy_file(AUDITDISTD_CONF,AUDITDISTD_BACKUP);
            // Copy original files to backup     
            copy_file(AUDIT_CONTROL,BACKUP_AUDIT_CONTROL); 
            copy_file(AUDIT_USER,BACKUP_AUDIT_USER);       
            copy_file(AUDIT_WARN,BACKUP_AUDIT_WARN);
        }else{
            // Copy backup to UHB
            copy_file(BACKUP_AUDIT_CONTROL,UHB_AUDIT_CONTROL); 
            copy_file(BACKUP_AUDIT_USER,UHB_AUDIT_USER);       
            copy_file(BACKUP_AUDIT_WARN,UHB_AUDIT_WARN); 
            if(remote_auditing_present)
                copy_file(AUDITDISTD_CONF,AUDITDISTD_BACKUP);
        }          
    }else if(aud_exists() && !check_auditing_status()){
        printf("WRN: Auditing daemon is detected, but not running!\n");
    }
}

void reset_auditing_configuration() {
    printf("MSG: Resetting auditing configuration...\n");
    copy_file(BACKUP_AUDIT_CONTROL,UHB_AUDIT_CONTROL); 
    copy_file(BACKUP_AUDIT_USER,UHB_AUDIT_USER);       
    copy_file(BACKUP_AUDIT_WARN,UHB_AUDIT_WARN);
}

void view_auditing_configuration(){
    int opt = three_option_input("MSG 1/3: Show audit_control file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(UHB_AUDIT_CONTROL);
    if(opt == 2)
         return;
    opt = three_option_input("MSG 2/3: Show audit_user file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(UHB_AUDIT_USER);
    if(opt == 2)
         return;
    opt = three_option_input("MSG 3/3: Show audit_warn file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(UHB_AUDIT_WARN);
    if(opt == 2)
         return;
}

void add_audit_control_option() {
    char param[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG: Add the new parameter for audit_control:",param,sizeof(param));
        opt = three_option_input("MSG: Is the parameter correct? (Y)es/(N)o/E(x)it:",'Y','N','X');
    }
    if(opt == 0){
        append_to_file(param,UHB_AUDIT_CONTROL);
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

void add_local_auditing() {
    int opt = three_option_input("MSG 1/3: Add rule to the audit_control file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        add_audit_control_option();
    if(opt == 2)
         return;
    opt = three_option_input("MSG 2/3: Add rule to the audit_user file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        add_audit_user_option();
    if(opt == 2)
         return;
    opt = three_option_input("MSG 3/3: Add rule to the audit_warn file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        add_audit_warn_option();
    if(opt == 2)
         return;
}

void configure_auditing_forwarding_service() {
    printf("MSG: SENDER SECTION:\n");
    if(smart_replacement("MSG 1/8: Source IP address for connections:",AUDITDISTD_CONF,20,"\"<addr>\"") == 0) return;
    if(smart_replacement("MSG 2/8: Directory with audit trail files in the host system:",AUDITDISTD_CONF,24,"\"<dir>\"") == 0) return;
    printf("MSG: TARGET SYSTEM CONFIGURATION SECTION:\n");
    if(smart_replacement("MSG 3/8: Hostname of the target system to send audit files to:",AUDITDISTD_CONF,28,"\"<name>\"") == 0) return;
    if(smart_replacement("MSG 4/8: Source IP address for connections:",AUDITDISTD_CONF,31,"\"<addr>\"") == 0) return;
    if(smart_replacement("MSG 5/8: IP address of the target system:",AUDITDISTD_CONF,35,"\"<addr>\"") == 0) return;
    if(smart_replacement("MSG 6/8: Directory with audit trail files in the target system",AUDITDISTD_CONF,39,"\"<dir>\"") == 0) return;
    if(smart_replacement("MSG 7/8: SHA256 Fingerprint of the receiver's public key:",AUDITDISTD_CONF,45,"\"<algorithm=hash>\"") == 0) return;
    if(smart_replacement("MSG 8/8: Password to authenticate in front of the receiver:",AUDITDISTD_CONF,48,"\"<password>\"") == 0) return;
}

void configure_auditing_reception_service() {
    printf("MSG: RECEIVER SECTION:\n");
    if(smart_replacement("MSG 1/8: Address to listen on <protocol>://<ip>:<port> :",AUDITDISTD_CONF,60,"\"<addr>\"") == 0) return;
    if(smart_replacement("MSG 2/8: Base directory",AUDITDISTD_CONF,66,"\"<basedir>\"") == 0) return;
    if(smart_replacement("MSG 3/8: Path to the receiver's certificate file:",AUDITDISTD_CONF,70,"\"<path>\"") == 0) return;
    if(smart_replacement("MSG 4/8: Path to the reciever's private key file:",AUDITDISTD_CONF,74,"\"<path>\"") == 0) return;
    printf("MSG: SOURCE SYSTEM CONFIGURATION SECTION:\n");
    if(smart_replacement("MSG 5/8: Hostname",AUDITDISTD_CONF,78,"\"<name>\"") == 0) return;
    if(smart_replacement("MSG 6/8: Sender IP address:",AUDITDISTD_CONF,81,"\"<addr>\"") == 0) return;
    if(smart_replacement("MSG 7/8: Directory where to store received audit trail files:",AUDITDISTD_CONF,86,"\"<dir>\"") == 0) return;
    if(smart_replacement("MSG 8/8: Password used by the sender to authenticate:",AUDITDISTD_CONF,89,"\"<password>\"") == 0) return;
}
