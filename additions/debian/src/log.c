#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "global_var.h"
#include "module_var.h"
#include "file.h"
#include "utils.h"
#include "input_output.h"
#include "log.h"

#define RSYSLOG_ORIGINAL_CONF "/etc/rsyslog.conf"
#define RSYSLOG_BACKUP_CONF "/root/uhb/base/config/template/rsyslog.conf.template"
#define RSYSLOG_REMOTE_CONF "/root/uhb/base/config/template/50-default.conf"

#define SEND_RFC5424 "$ActionForwardDefaultTemplate RSYSLOG_SyslogProtocol23Format"
#define WRITE_RFC5424 "$ActionFileDefaultTemplate RSYSLOG_SyslogProtocol23Format"

static bool rfc_5424;

static char udp_port[10] = "514";   // Port for the UDP listening module - port 514 as startup default
static char tcp_port[10] = "514";   // Port for the TCP listening module - port 514 as startup default

bool log_exists() {
    if(exec_exists("rsyslog")){
        printf("MSG: Logging daemon was detected.\n");
        return true;
    }else{
        printf("MSG: Logging daemon was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

bool check_logging_status(){
    char command[MAX_LINE_LENGTH];
    snprintf(command, sizeof(command), "service rsyslog status >/dev/null 2>&1");
    if(system(command) == 0){
        printf("MSG: Rsyslog daemon is running.\n");
        return true;
    }else{
        printf("MSG: Rsyslog daemon is NOT running.\n");
        return false;
    }
}

void detect_rfc5424(){
    if(find_string_in_file(SEND_RFC5424,RSYSLOG_ORIGINAL_CONF) && find_string_in_file(WRITE_RFC5424,RSYSLOG_ORIGINAL_CONF)){
        printf("MSG: RFC5424 standard detected.\n");
        rfc_5424 = true;
    }else{
        printf("MSG: RFC3164 standard detected.\n");
        rfc_5424 = false;
    }
}

void apply_rfc5424(){
    int opt = get_yes_no_input("Use RFC5424 standard when sending and receving logs? (Y/N)?\n");
    if(opt == 0){
        append_to_file(SEND_RFC5424,CONFIG_LOG);
        append_to_file(WRITE_RFC5424,CONFIG_LOG);
        rfc_5424 = true;
    }else if(opt == 1){
        find_and_replace(SEND_RFC5424,"",CONFIG_LOG);
        find_and_replace(WRITE_RFC5424,"",CONFIG_LOG);
        rfc_5424 = false;
    }
}

bool restart_logging_daemon(){
    char command[MAX_LINE_LENGTH];
    printf("MSG: Restarting rsyslog daemon...\n");
    snprintf(command, sizeof(command), "service rsyslog restart");
    if(system(command) == 0){
        printf("MSG: Rsyslog daemon successfully restarted.\n");
        return true;
    }else{
        printf("WRN: Unable to restart rsyslog daemon.\n");
        return false;
    }
}

void initialize_logging(){
    if(log_exists() && check_logging_status()){
        detect_rfc5424();
        copy_file(RSYSLOG_ORIGINAL_CONF,CONFIG_LOG);            // Copy to the log configuration
        copy_file(RSYSLOG_ORIGINAL_CONF,RSYSLOG_BACKUP_CONF);   // Backup the original configuration just in case
    }else if(log_exists() && !check_logging_status()){
        printf("WRN: Rsyslog daemon is detected, but not running!\n");
    }
}

bool apply_logging_config(){
    printf("MSG: Applying UHB logging configuration...\n");
    if(copy_file(CONFIG_LOG,RSYSLOG_ORIGINAL_CONF)){
        if(restart_logging_daemon()){
            printf("MSG: UHB logging configuration successfully applied.\n");
            return true;
        }else{
            fprintf(stderr, "ERR: apply_logging_config(): Could not restart logging daemon.\n");
            return false;
        }
    }else{
        fprintf(stderr, "ERR: apply_logging_config(): Could not apply configuration file.\n");
        return false;
    }
}

// View logging configuration is already implemented in the menu function

void enable_udp_module(){
    find_and_replace("#module(load=\"imudp\")","module(load=\"imudp\")",CONFIG_LOG);
    find_and_replace("#input(type=\"imudp\" port=\"514\")","input(type=\"imudp\" port=\"514\")",CONFIG_LOG);
}

void enable_tcp_module(){
    find_and_replace("#module(load=\"imtcp\")","module(load=\"imtcp\")",CONFIG_LOG);
    find_and_replace("#input(type=\"imtcp\" port=\"514\")","input(type=\"imtcp\" port=\"514\")",CONFIG_LOG);
}

bool edit_udp_module(const char *port){
    char current[MAX_LINE_LENGTH];
    char replace[MAX_LINE_LENGTH];
    if(snprintf(current,sizeof(current),"#input(type=\"imudp\" port=\"%s\")",udp_port) < 0){
        fprintf(stderr, "ERR: edit_udp_module(): Could not retrieve current UDP module info.\n");
        return false;
    }
    if(snprintf(replace,sizeof(replace),"#input(type=\"imudp\" port=\"%s\")",port) < 0){
        fprintf(stderr, "ERR: edit_udp_module(): Could not retrieve replacement UDP module info.\n");
        return false;
    }
    return find_and_replace(current,replace,CONFIG_LOG);
}

bool edit_tcp_module(const char *port){
    char current[MAX_LINE_LENGTH];
    char replace[MAX_LINE_LENGTH];
    if(snprintf(current,sizeof(current),"#input(type=\"imtcp\" port=\"%s\")",tcp_port) < 0){
        fprintf(stderr, "ERR: edit_tcp_module(): Could not retrieve current TCP module info.\n");
        return false;
    }
    if(snprintf(replace,sizeof(replace),"#input(type=\"imtcp\" port=\"%s\")",port) < 0){
        fprintf(stderr, "ERR: edit_tcp_module(): Could not retrieve replacement TCP module info.\n");
        return false;
    }
    return find_and_replace(current,replace,CONFIG_LOG);
}

void set_log_server(){
    char port[10];
    if(get_yes_no_input("MSG: Would you like to listen for incoming UDP log messages?\n") == 0){
        while(!is_port_open(atoi(port))){
            get_user_input("Which port would you like to use?:\n",port,sizeof(port));
        }
        edit_udp_module(port);
    }
    if(get_yes_no_input("MSG: Would you like to listen for incoming TCP log messages?\n") == 0){
        while(!is_port_open(atoi(port))){
            get_user_input("Which port would you like to use?:\n",port,sizeof(port));
        }
        edit_tcp_module(port);
    }
}

void set_log_server_location(){
    char filename[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    get_user_input("Where would you like to store incoming log messages?\n",filename,sizeof(filename));
    snprintf(line,sizeof(line),"$template RemoteLogs,\"%s\"",filename);
    append_to_file(line,CONFIG_LOG);
    append_to_file("*.* ?RemoteLogs",CONFIG_LOG);
    append_to_file("& ~",CONFIG_LOG);
}

