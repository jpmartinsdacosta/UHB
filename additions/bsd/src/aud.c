#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "global_var.h"
#include "module_var.h"
#include "file.h"
#include "utils.h"
#include "input_output.h"
#include "aud.h"

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Filepath to the original configuration files.
#define BSD_AUDIT_CONTROL_ORIGINAL      "/etc/security/audit_control"
#define BSD_AUDIT_USER_ORIGINAL         "/etc/security/audit_user"
#define BSD_AUDIT_WARN_ORIGINAL         "/etc/security/audit_warn"
#define BSD_AUDIT_AUDITDISTD_ORIGINAL   "/etc/security/auditdistd.conf"

// Filepath to the configuration files to be used/edited in UHB.
#define BSD_AUDIT_CONTROL_CURRENT       "/root/uhb/base/config/current/audit_control"
#define BSD_AUDIT_USER_CURRENT          "/root/uhb/base/config/current/audit_user"
#define BSD_AUDIT_WARN_CURRENT          "/root/uhb/base/config/current/audit_warn"
#define BSD_AUDIT_AUDITDISTD_CURRENT    "/root/uhb/base/config/current/auditdistd.conf"

// Filepath to the backup of all configuration files.
#define BSD_AUDIT_CONTROL_BACKUP        "/root/uhb/base/config/backups/audit_control"
#define BSD_AUDIT_USER_BACKUP           "/root/uhb/base/config/backups/audit_user"    
#define BSD_AUDIT_WARN_BACKUP           "/root/uhb/base/config/backups/audit_warn"
#define BSD_AUDIT_AUDITDISTD_BACKUP     "/root/uhb/base/config/backups/auditdistd.conf"

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

bool remote_auditing_exists(){
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
        printf("ERR: Auditing daemon must be manually enabled in rc.conf.\n");
        return false;
    }
}

bool restart_remote_auditing_service() {
    char command[MAX_LINE_LENGTH];
    printf("MSG: Restarting auditdistd...\n");
    if(enabled_in_rc_conf("auditd_enable")){
        snprintf(command, sizeof(command), "service auditdistd restart >/dev/null 2>&1");
        if(system(command) == 0){
            printf("MSG: Auditdistd successfully restarted.\n");
            return true;
        }else{
            printf("WRN: Unable to restart auditdistd.\n");
            return false;
        }
    }else{
        printf("ERR: Auditdistd must be mannualy enabled in rc.conf.\n");
        return false;
    }
}

/**
 * Functions regarding module configuration
 */

void initialize_auditing(bool copy_from_backup) {
    if(aud_exists() && check_auditing_status()){
        if(!copy_from_backup){
            copy_file(BSD_AUDIT_CONTROL_ORIGINAL,BSD_AUDIT_CONTROL_CURRENT); 
            copy_file(BSD_AUDIT_USER_ORIGINAL,BSD_AUDIT_USER_CURRENT);       
            copy_file(BSD_AUDIT_WARN_ORIGINAL,BSD_AUDIT_WARN_CURRENT); 
            if(remote_auditing_present)
                copy_file(BSD_AUDIT_AUDITDISTD_ORIGINAL,BSD_AUDIT_AUDITDISTD_BACKUP);    
            copy_file(BSD_AUDIT_CONTROL_ORIGINAL,BSD_AUDIT_CONTROL_BACKUP); 
            copy_file(BSD_AUDIT_USER_ORIGINAL,BSD_AUDIT_USER_BACKUP);       
            copy_file(BSD_AUDIT_WARN_ORIGINAL,BSD_AUDIT_WARN_BACKUP);
            if(remote_auditing_present)
                copy_file(BSD_AUDIT_AUDITDISTD_ORIGINAL,BSD_AUDIT_AUDITDISTD_BACKUP);
        }else{
            copy_file(BSD_AUDIT_CONTROL_BACKUP,BSD_AUDIT_CONTROL_CURRENT); 
            copy_file(BSD_AUDIT_USER_BACKUP,BSD_AUDIT_USER_CURRENT);       
            copy_file(BSD_AUDIT_WARN_BACKUP,BSD_AUDIT_WARN_CURRENT); 
            if(remote_auditing_present)
                copy_file(BSD_AUDIT_AUDITDISTD_BACKUP,BSD_AUDIT_AUDITDISTD_CURRENT);
        }          
    }else if(aud_exists() && !check_auditing_status()){
        printf("WRN: Auditing daemon is detected, but not running!\n");
    }
}

void reset_auditing_configuration() {
    printf("MSG: Resetting auditing configuration...\n");
    copy_file(BSD_AUDIT_CONTROL_BACKUP,BSD_AUDIT_CONTROL_CURRENT); 
    copy_file(BSD_AUDIT_USER_BACKUP,BSD_AUDIT_USER_CURRENT);       
    copy_file(BSD_AUDIT_WARN_BACKUP,BSD_AUDIT_WARN_CURRENT);
    copy_file(BSD_AUDIT_AUDITDISTD_BACKUP,BSD_AUDIT_AUDITDISTD_CURRENT);
}

void view_auditing_configuration(){
    int opt = three_option_input("MSG 1/4: View audit_control file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(BSD_AUDIT_CONTROL_CURRENT);
    if(opt == 2)
         return;
    opt = three_option_input("MSG 2/4: View audit_user file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(BSD_AUDIT_USER_CURRENT);
    if(opt == 2)
         return;
    opt = three_option_input("MSG 3/4: View audit_warn file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(BSD_AUDIT_WARN_CURRENT);
    if(opt == 2)
         return;
    opt = three_option_input("MSG 4/4: View auditdistd.conf file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(BSD_AUDIT_AUDITDISTD_CURRENT);
    if(opt == 2)
         return;
}

void apply_auditing_configuration() {
    if(get_yes_no_input("MSG: Apply the current auditing configuration? (Y/N):") == 0){
        printf("MSG: Applying auditing configuration...\n");
        if(copy_file(BSD_AUDIT_CONTROL_CURRENT,BSD_AUDIT_CONTROL_ORIGINAL) && copy_file(BSD_AUDIT_USER_CURRENT,BSD_AUDIT_USER_ORIGINAL) && copy_file(BSD_AUDIT_WARN_CURRENT,BSD_AUDIT_WARN_ORIGINAL)){
            if(restart_auditing_daemon()){
                printf("MSG: UHB auditing configuration successfully applied.\n");
            }else{
                fprintf(stderr, "ERR: apply_auditing_configuration(): Could not restart auditing daemon.\n");
            }
            if(remote_auditing_exists()){
                if(copy_file(BSD_AUDIT_AUDITDISTD_CURRENT,BSD_AUDIT_AUDITDISTD_ORIGINAL)){
                    if(restart_remote_auditing_service()){
                        printf("MSG: Remote UHB auditing configuration successfully applied.\n");
                    }else{
                        fprintf(stderr, "ERR: apply_auditing_configuration(): Could not restart remote auditing daemon.\n");
                    }
                }else{
                    fprintf(stderr, "ERR: apply_auditing_configuration(): Could not apply remote auditing configuration file.\n");
                }
            }
        }else{
            fprintf(stderr, "ERR: apply_auditing_configuration(): Could not apply configuration file.\n");
        }
    } 
}

void add_audit_control_option() {
    char param[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG: Add the new parameter for audit_control:",param,sizeof(param));
        opt = three_option_input("MSG: Is the parameter correct? (Y)es/(N)o/E(x)it:",'Y','N','X');
    }
    if(opt == 0){
        append_to_file(param,BSD_AUDIT_CONTROL_CURRENT);
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
        append_to_file(param,BSD_AUDIT_USER_CURRENT);
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
        append_to_file(param,BSD_AUDIT_WARN_CURRENT);
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
    if(smart_replacement("MSG 1/8: Source IP address for connections:",BSD_AUDIT_AUDITDISTD_CURRENT,20,"\"<addr>\"") == 0) return;
    if(smart_replacement("MSG 2/8: Directory with audit trail files in the host system:",BSD_AUDIT_AUDITDISTD_CURRENT,24,"\"<dir>\"") == 0) return;
    printf("MSG: TARGET SYSTEM CONFIGURATION SECTION:\n");
    if(smart_replacement("MSG 3/8: Hostname of the target system to send audit files to:",BSD_AUDIT_AUDITDISTD_CURRENT,28,"\"<name>\"") == 0) return;
    if(smart_replacement("MSG 4/8: Source IP address for connections:",BSD_AUDIT_AUDITDISTD_CURRENT,31,"\"<addr>\"") == 0) return;
    if(smart_replacement("MSG 5/8: IP address of the target system:",BSD_AUDIT_AUDITDISTD_CURRENT,35,"\"<addr>\"") == 0) return;
    if(smart_replacement("MSG 6/8: Directory with audit trail files in the target system",BSD_AUDIT_AUDITDISTD_CURRENT,39,"\"<dir>\"") == 0) return;
    if(smart_replacement("MSG 7/8: SHA256 Fingerprint of the receiver's public key:",BSD_AUDIT_AUDITDISTD_CURRENT,45,"\"<algorithm=hash>\"") == 0) return;
    if(smart_replacement("MSG 8/8: Password to authenticate in front of the receiver:",BSD_AUDIT_AUDITDISTD_CURRENT,48,"\"<password>\"") == 0) return;
}

void configure_auditing_reception_service() {
    printf("MSG: RECEIVER SECTION:\n");
    if(smart_replacement("MSG 1/8: Address to listen on <protocol>://<ip>:<port> :",BSD_AUDIT_AUDITDISTD_CURRENT,60,"\"<addr>\"") == 0) return;
    if(smart_replacement("MSG 2/8: Base directory",BSD_AUDIT_AUDITDISTD_CURRENT,66,"\"<basedir>\"") == 0) return;
    if(smart_replacement("MSG 3/8: Path to the receiver's certificate file:",BSD_AUDIT_AUDITDISTD_CURRENT,70,"\"<path>\"") == 0) return;
    if(smart_replacement("MSG 4/8: Path to the reciever's private key file:",BSD_AUDIT_AUDITDISTD_CURRENT,74,"\"<path>\"") == 0) return;
    printf("MSG: SOURCE SYSTEM CONFIGURATION SECTION:\n");
    if(smart_replacement("MSG 5/8: Hostname",BSD_AUDIT_AUDITDISTD_CURRENT,78,"\"<name>\"") == 0) return;
    if(smart_replacement("MSG 6/8: Sender IP address:",BSD_AUDIT_AUDITDISTD_CURRENT,81,"\"<addr>\"") == 0) return;
    if(smart_replacement("MSG 7/8: Directory where to store received audit trail files:",BSD_AUDIT_AUDITDISTD_CURRENT,86,"\"<dir>\"") == 0) return;
    if(smart_replacement("MSG 8/8: Password used by the sender to authenticate:",BSD_AUDIT_AUDITDISTD_CURRENT,89,"\"<password>\"") == 0) return;
}

/**
 * Functions regarding manuals
 */

void view_auditing_manual() {
    system("man auditd");
    system("clear");
}

void view_remote_auditing_manual() {
    system("man auditdistd");
    system("clear");
}
