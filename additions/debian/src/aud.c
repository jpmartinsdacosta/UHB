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

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Filepath to the original configuration files.
#define DEB_AUDIT_CONF_ORIGINAL     "/etc/audit/auditd.conf"
#define DEB_AUDIT_RULES_ORIGINAL    "/etc/audit/audit.rules"

// Filepath to the configuration files to be used/edited in UHB.
#define DEB_AUDIT_CONF_CURRENT      "/root/uhb/base/config/current/auditd.conf" 
#define DEB_AUDIT_RULES_CURRENT     "/root/uhb/base/config/current/audit.rules" 

// Filepath to the backup of all configuration files.
#define DEB_AUDIT_CONF_BACKUP       "/root/uhb/base/config/backups/auditd.conf"
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

bool remote_auditing_daemon_exists(){return aud_exists();}

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
            copy_file(DEB_AUDIT_RULES_ORIGINAL,DEB_AUDIT_RULES_CURRENT);
            // Copy original files to backup
            copy_file(DEB_AUDIT_CONF_ORIGINAL,DEB_AUDIT_CONF_BACKUP);
            copy_file(DEB_AUDIT_RULES_ORIGINAL,DEB_AUDIT_RULES_BACKUP);
        }else{
            // Copy backup to UHB
            copy_file(DEB_AUDIT_CONF_BACKUP,DEB_AUDIT_CONF_CURRENT);
            copy_file(DEB_AUDIT_RULES_BACKUP,DEB_AUDIT_RULES_CURRENT);
        }
    }
}

void reset_auditing_configuration() {
    printf("MSG: Resetting auditing configuration...\n");
    // Copy backup to UHB
    copy_file(DEB_AUDIT_CONF_BACKUP,DEB_AUDIT_CONF_CURRENT);
    copy_file(DEB_AUDIT_RULES_BACKUP,DEB_AUDIT_RULES_CURRENT);
}

void view_auditing_configuration() {
    int opt = three_option_input("MSG 1/2: View auditd.conf file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(DEB_AUDIT_CONF_CURRENT);
    if(opt == 2)
         return;
    opt = three_option_input("MSG 2/2: View audit.rules file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
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
    if(get_yes_no_input("MSG: Apply the current auditing configuration? (Y/N):")){
        printf("MSG: Applying auditing configuration...\n");
        // Copy UHB files to original location and reset auditing service
        copy_file(DEB_AUDIT_CONF_CURRENT,DEB_AUDIT_CONF_ORIGINAL);
        copy_file(DEB_AUDIT_RULES_CURRENT,DEB_AUDIT_RULES_ORIGINAL);
        // Load rules from the audit.rules file
        system("augenrules --load");
        // Restart auditing daemon
        restart_auditing_daemon();
    }
}

/**
 * Functions regarding the reception and forwarding of audit messages to the network.
 */

void configure_auditing_reception_service() {
    char param[MAX_LINE_LENGTH];
    get_user_input("MSG: Which TCP port should the auditing daemon listen to?:",param,sizeof(param));
    if(!find_string_in_file("tcp_listen_port",DEB_AUDIT_CONF_CURRENT))
        replace_string_in_line(DEB_AUDIT_CONF_CURRENT,27,"##tcp_listen_port","tcp_listen_port");
    replace_option_value("tcp_listen_port",'=',param,DEB_AUDIT_CONF_CURRENT);
    if(!find_string_in_file("tcp_client_ports",DEB_AUDIT_CONF_CURRENT))
        replace_string_in_line(DEB_AUDIT_CONF_CURRENT,27,"##tcp_client_ports","tcp_client_ports");
    replace_option_value("tcp_client_ports",'=',param,DEB_AUDIT_CONF_CURRENT);
}

void configure_auditing_forwarding_service() {
    char param[MAX_LINE_LENGTH]; 
    get_user_input("MSG: Please enter the IP address of the remote server:",param,sizeof(param));
    if(!find_string_in_file("remote_server",DEB_AUDIT_CONF_CURRENT)){
        char line[MAX_LINE_LENGTH];
        snprintf(line,sizeof(line),"remote_server = %s",param);
        append_to_file(line,DEB_AUDIT_CONF_CURRENT);
    }
    replace_option_value("remote_server",'=',param,DEB_AUDIT_CONF_CURRENT);
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
