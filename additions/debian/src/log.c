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
#define RSYSLOG_FORWARD_CONF "/etc/rsyslog.d/50-default.conf"
#define RSYSLOG_BACKUP_CONF "/root/uhb/base/config/template/rsyslog.conf.template"
#define RSYSLOG_REMOTE_CONF "/root/uhb/base/config/template/50-default.conf"

#define SEND_RFC5424 "$ActionForwardDefaultTemplate RSYSLOG_SyslogProtocol23Format\n"
#define WRITE_RFC5424 "$ActionFileDefaultTemplate RSYSLOG_SyslogProtocol23Format\n"

static bool rfc_5424_send;
static bool rfc_5424_write;

static char udp_port[10] = "514";   // Port for the UDP listening module - port 514 as startup default
static char tcp_port[10] = "514";   // Port for the TCP listening module - port 514 as startup default

/**
 * Functions to check the existence and status of rsyslog
 */

bool log_exists() {
    if(exec_exists("rsyslogd")){
        printf("MSG: Logging daemon was detected.\n");
        return true;
    }else{
        printf("MSG: Logging daemon was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

bool check_logging_status() {
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

bool restart_logging_daemon() {
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

/**
 * Functions regarding the RFC protocol being used.
 */

void detect_rfc5424() {
    if(find_string_in_file(SEND_RFC5424,CONFIG_LOG)){
        rfc_5424_send = true;
    }else{
        rfc_5424_send = false;
    }
    if(find_string_in_file(WRITE_RFC5424,CONFIG_LOG)){
        rfc_5424_write = true;
    }else{
        rfc_5424_write = false;
    }
}

void apply_rfc5424() {
    int opt = get_yes_no_input("MSG: Use RFC5424 standard for remote logs? (Y/N)?\n");
    if(opt == 0){
        append_to_file(SEND_RFC5424,CONFIG_LOG);
        rfc_5424_send = true;
    }else if(opt == 1){
        find_and_replace(SEND_RFC5424,"",CONFIG_LOG);
        rfc_5424_send = false;
    }
    opt = get_yes_no_input("MSG: Use RFC5424 standard when storing logs in the system? (Y/N)?\n");
    if(opt == 0){
        append_to_file(WRITE_RFC5424,CONFIG_LOG);
        rfc_5424_write = true;
    }else if(opt == 1){
        find_and_replace(WRITE_RFC5424,"",CONFIG_LOG);
        rfc_5424_write = false;
    }
}

void enable_udp_module(){
    find_and_replace("#module(load=\"imudp\")","module(load=\"imudp\")",CONFIG_LOG);
    find_and_replace("#input(type=\"imudp\" port=\"514\")","input(type=\"imudp\" port=\"514\")",CONFIG_LOG);
}

void enable_tcp_module(){
    find_and_replace("#module(load=\"imtcp\")","module(load=\"imtcp\")",CONFIG_LOG);
    find_and_replace("#input(type=\"imtcp\" port=\"514\")","input(type=\"imtcp\" port=\"514\")",CONFIG_LOG);
}

void initialize_logging(){
    if(log_exists() && check_logging_status()){
        copy_file(RSYSLOG_ORIGINAL_CONF,CONFIG_LOG);            // Copy to the log configuration
        copy_file(RSYSLOG_ORIGINAL_CONF,RSYSLOG_BACKUP_CONF);   // Backup the original configuration just in case
        detect_rfc5424();
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

/**
 * Functions to manage logging inside the system
 */

 void add_local_logs() {
    char msg[MAX_LINE_LENGTH];
    char fp[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG: Please enter the messages you want to log:\n",msg,sizeof(msg));
        opt = get_yes_no_input("MSG: Is the information correct? (Y/N)\n");
    }
    opt = 1;
    while(opt == 1){
        get_user_input("MSG: Please enter where these log messages will be stored:\n",msg,sizeof(msg));
        opt = get_yes_no_input("MSG: Is the information correct? (Y/N)\n");
    }
    snprintf(command,sizeof(command),"%s    %s\n");
    append_to_file(command,CONFIG_LOG);
    if(!path_exists(fp)){
        return create_file(fp);
    }else{
        return true;
    }
 }

/**
 * Functions for remote logging/log forwarding
 */

bool edit_udp_module(const char *port){
    char current[MAX_LINE_LENGTH];
    char replace[MAX_LINE_LENGTH];
    if(snprintf(current,sizeof(current),"input(type=\"imudp\" port=\"%s\")",udp_port) < 0){
        fprintf(stderr, "ERR: edit_udp_module(): Could not retrieve current UDP module info.\n");
        return false;
    }
    if(snprintf(replace,sizeof(replace),"input(type=\"imudp\" port=\"%s\")",port) < 0){
        fprintf(stderr, "ERR: edit_udp_module(): Could not retrieve replacement UDP module info.\n");
        return false;
    }
    return find_and_replace(current,replace,CONFIG_LOG);
}

bool edit_tcp_module(const char *port){
    char current[MAX_LINE_LENGTH];
    char replace[MAX_LINE_LENGTH];
    if(snprintf(current,sizeof(current),"input(type=\"imtcp\" port=\"%s\")",tcp_port) < 0){
        fprintf(stderr, "ERR: edit_tcp_module(): Could not retrieve current TCP module info.\n");
        return false;
    }
    if(snprintf(replace,sizeof(replace),"input(type=\"imtcp\" port=\"%s\")",port) < 0){
        fprintf(stderr, "ERR: edit_tcp_module(): Could not retrieve replacement TCP module info.\n");
        return false;
    }
    return find_and_replace(current,replace,CONFIG_LOG);
}

void set_log_reception_service(){
    char port[10];
    if(get_yes_no_input("MSG: Would you like to listen for receiving UDP log messages?\n") == 0){
        while(!is_port_open(atoi(port))){
            get_user_input("MSG: Which port would you like to use?:\n",port,sizeof(port));
        }
        edit_udp_module(port);
    }
    if(get_yes_no_input("MSG: Would you like to listen for receving TCP log messages?\n") == 0){
        while(!is_port_open(atoi(port))){
            get_user_input("MSG: Which port would you like to use?:\n",port,sizeof(port));
        }
        edit_tcp_module(port);
    }
}

void add_log_reception_rule(){
    char filename[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    get_user_input("MSG: Where would you like to store receving log messages?\n",filename,sizeof(filename));
    snprintf(line,sizeof(line),"$template RemoteLogs,\"%s\"",filename);
    append_to_file(line,CONFIG_LOG);
    append_to_file("*.* ?RemoteLogs",CONFIG_LOG);
    append_to_file("& ~",CONFIG_LOG);
}

void add_log_forwarding_rule(){
    char command[MAX_LINE_LENGTH];
    char ip[20];
    char port[10];
    char directive[MAX_LINE_LENGTH];
    get_user_input("MSG: Please enter the remote server IP address:\n",ip,sizeof(ip));
    get_user_input("MSG: Please enter the port number to be used:\n",port,sizeof(port));
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG: Please add additional rsyslog file and filter directives:\n",directive,sizeof(directive));
        opt = get_yes_no_input("MSG: Are the added directives correct? (Y/N):\n");
    }
    opt = three_option_input("MSG: Is the remote server using (U)DP or (T)CP? X to E(x)it.\n",'U','T','X');
    switch (opt) {
        case 0:
            snprintf(command,sizeof(command),"%s @%s:%s\n",directive,ip,port);
            break;
        case 1:
            snprintf(command,sizeof(command),"%s @@%s:%s\n",directive,ip,port);
            break;
        case 2:
            return;
            break;
    }
    append_to_file(command,RSYSLOG_FORWARD_CONF);
}
