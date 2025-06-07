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
#define DEB_AUDIT_CONF_ORIGINAL     "/etc/audit/auditd.conf"
#define DEB_AUDIT_AUDISP_ORIGINAL   "/etc/audit/audisp-remote.conf"
#define DEB_AUDIT_REMOTE_ORIGINAL   "/etc/audit/plugins.d/au-remote.conf"
#define DEB_AUDIT_RULES_ORIGINAL    "/etc/audit/audit.rules"

// Filepath to the configuration files to be used/edited in UHB.
#define DEB_AUDIT_CONF_CURRENT      "/root/uhb/base/config/current/auditd.conf" 
#define DEB_AUDIT_AUDISP_CURRENT    "/root/uhb/base/config/current/audisp-remote.conf"
#define DEB_AUDIT_REMOTE_CURRENT    "/root/uhb/base/config/current/au-remote.conf"
#define DEB_AUDIT_RULES_CURRENT     "/root/uhb/base/config/current/audit.rules" 

// Filepath to the backup of all configuration files.
#define DEB_AUDIT_CONF_BACKUP       "/root/uhb/base/config/backups/auditd.conf"
#define DEB_AUDIT_AUDISP_BACKUP     "/root/uhb/base/config/backups/audisp-remote.conf"
#define DEB_AUDIT_REMOTE_BACKUP     "/root/uhb/base/config/backups/au-remote.conf"
#define DEB_AUDIT_RULES_BACKUP      "/root/uhb/base/config/backups/audit.rules"


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
    return (path_exists(DEB_AUDIT_AUDISP_CURRENT) && path_exists(DEB_AUDIT_REMOTE_CURRENT));
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
    snprintf(command, sizeof(command), "service auditd restart >/dev/null 2>&1");
    if(system(command) == 0){
        printf("MSG: Auditing daemon successfully restarted.\n");
        return true;
    }else{
        printf("WRN: Unable to restart auditing daemon.\n");
        return false;
    }
}

/**
 * Functions regarding module configuration
 */

void initialize_auditing(bool copy_from_backup){
    if(aud_exists() && check_auditing_status()){
        if(!copy_from_backup){
            // Copy original files to UHB
            copy_file(DEB_AUDIT_CONF_ORIGINAL,DEB_AUDIT_CONF_CURRENT);
            copy_file(DEB_AUDIT_AUDISP_ORIGINAL,DEB_AUDIT_AUDISP_CURRENT);
            copy_file(DEB_AUDIT_REMOTE_ORIGINAL,DEB_AUDIT_REMOTE_CURRENT);
            copy_file(DEB_AUDIT_RULES_ORIGINAL,DEB_AUDIT_RULES_CURRENT);
            // Copy original files to backup
            copy_file(DEB_AUDIT_CONF_ORIGINAL,DEB_AUDIT_CONF_BACKUP);
            copy_file(DEB_AUDIT_AUDISP_ORIGINAL,DEB_AUDIT_AUDISP_BACKUP);
            copy_file(DEB_AUDIT_REMOTE_ORIGINAL,DEB_AUDIT_REMOTE_BACKUP);
            copy_file(DEB_AUDIT_RULES_ORIGINAL,DEB_AUDIT_RULES_BACKUP);
        }else{
            // Copy backup to UHB
            copy_file(DEB_AUDIT_CONF_BACKUP,DEB_AUDIT_CONF_CURRENT);
            copy_file(DEB_AUDIT_AUDISP_BACKUP,DEB_AUDIT_AUDISP_CURRENT);
            copy_file(DEB_AUDIT_REMOTE_BACKUP,DEB_AUDIT_REMOTE_CURRENT);
            copy_file(DEB_AUDIT_RULES_BACKUP,DEB_AUDIT_RULES_CURRENT);
        }
    }
}

void reset_auditing_configuration() {
    printf("MSG: Resetting auditing configuration...\n");
    copy_file(DEB_AUDIT_CONF_BACKUP,DEB_AUDIT_CONF_CURRENT);
    copy_file(DEB_AUDIT_AUDISP_BACKUP,DEB_AUDIT_AUDISP_CURRENT);
    copy_file(DEB_AUDIT_REMOTE_BACKUP,DEB_AUDIT_REMOTE_CURRENT);
    copy_file(DEB_AUDIT_RULES_BACKUP,DEB_AUDIT_RULES_CURRENT);
}

void view_auditing_configuration() {
    int opt = three_option_input("MSG 1/4: View auditd.conf file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(DEB_AUDIT_CONF_CURRENT);
    if(opt == 2)
         return;
    opt = three_option_input("MSG 2/4: View audisp-remote.conf file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(DEB_AUDIT_AUDISP_CURRENT);
    if(opt == 2)
         return;
    opt = three_option_input("MSG 3/4: View au-remote.conf file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(DEB_AUDIT_REMOTE_CURRENT);
    if(opt == 2)
         return;
    opt = three_option_input("MSG 4/4: View audit.rules file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(DEB_AUDIT_RULES_CURRENT);
    if(opt == 2)
         return;
}

void add_local_auditing() {
    char rule[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG: Please enter the auditing rule to be added:",rule,sizeof(rule));
        opt = three_option_input("Is the information correct? (Y)es/(N)o/E(x)it",'Y','N','X');
    }
    if(opt == 0) {
        append_to_file(rule,DEB_AUDIT_RULES_CURRENT);
    }

}

void apply_auditing_configuration() {
    if(get_yes_no_input("MSG: Apply the current auditing configuration? (Y/N):") == 0){
        printf("MSG: Applying UHB auditing configuration...\n");
        if(copy_file(DEB_AUDIT_CONF_CURRENT,DEB_AUDIT_CONF_ORIGINAL) && 
        copy_file(DEB_AUDIT_AUDISP_CURRENT,DEB_AUDIT_AUDISP_ORIGINAL) &&
        copy_file(DEB_AUDIT_REMOTE_CURRENT,DEB_AUDIT_REMOTE_ORIGINAL) &&
        copy_file(DEB_AUDIT_RULES_CURRENT,DEB_AUDIT_RULES_ORIGINAL)){
            if(system("augenrules --load") == 0 && restart_auditing_daemon()){
                printf("MSG: UHB auditing configuration successfully applied.\n");
            }else{
                fprintf(stderr, "ERR: apply_auditing_configuration(): Could not restart auditing daemon.\n");
            }
        }else{
            fprintf(stderr, "ERR: apply_auditing_configuration(): Could not apply configuration file.\n");
        }
    }
}

/**
 * Functions regarding the reception and forwarding of audit messages to the network.
 */

void configure_auditing_reception_service() {
    char ip[MAX_LINE_LENGTH];
    char port[MAX_LINE_LENGTH];
    int opt = 1;
    bool forward = false;
    while(opt == 1){
        if(get_yes_no_input("MSG 1/3: Enable remote auditing? (Y/N):") == 0){
            forward = true;
            get_user_input("MSG 2/3: Enter remote server IP:",ip,sizeof(ip));
            get_user_input("MSG 3/3: Enter remote server port:",port,sizeof(port));
        }else{
            forward = false;
        }
        opt = three_option_input("MSG: Is the provided information correct? (Y)es/(N)o/E(x)it:", 'Y', 'N', 'X');
    }
    if(forward && opt == 0) {
        replace_option_value("active",'=',"yes",DEB_AUDIT_REMOTE_CURRENT);
        replace_option_value("direction",'=',"in",DEB_AUDIT_REMOTE_CURRENT);
        if(is_valid_ipv4)
            replace_option_value("remote_server",'=',ip,DEB_AUDIT_AUDISP_CURRENT);
        if(is_valid_port(port))
            replace_option_value("port",'=',port,DEB_AUDIT_AUDISP_CURRENT);
    }else if(!forward && opt == 0) {
        replace_option_value("active",'=',"no",DEB_AUDIT_AUDISP_CURRENT);
        replace_option_value("direction",'=',"",DEB_AUDIT_AUDISP_CURRENT);
    }
}

void configure_auditing_forwarding_service() {
    char ip[MAX_LINE_LENGTH];
    char port[MAX_LINE_LENGTH];
    int opt = 1;
    bool forward = false;
    while(opt == 1){
        if(get_yes_no_input("MSG 1/3: Enable remote auditing? (Y/N):") == 0){
            forward = true;
            get_user_input("MSG 2/3: Enter remote server IP:",ip,sizeof(ip));
            get_user_input("MSG 3/3: Enter remote server port:",port,sizeof(port));
        }else{
            forward = false;
        }
        opt = three_option_input("MSG: Is the provided information correct? (Y)es/(N)o/E(x)it:", 'Y', 'N', 'X');
    }
    if(forward && opt == 0) {
        replace_option_value("active",'=',"yes",DEB_AUDIT_REMOTE_CURRENT);
        replace_option_value("direction",'=',"out",DEB_AUDIT_REMOTE_CURRENT);
        if(is_valid_ipv4)
            replace_option_value("remote_server",'=',ip,DEB_AUDIT_AUDISP_CURRENT);
        if(is_valid_port(port))
            replace_option_value("port",'=',port,DEB_AUDIT_AUDISP_CURRENT);
    }else if(!forward && opt == 0) {
        replace_option_value("active",'=',"no",DEB_AUDIT_REMOTE_CURRENT);
    }
}

/**
 * Functions regarding manuals
 */

void view_auditing_manual() {
    system("man audit.rules");
    system("clear");
}

void view_remote_auditing_manual() {
    system("man auditd.conf");
    system("clear");
}
