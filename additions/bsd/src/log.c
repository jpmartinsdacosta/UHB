#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <string.h>

#include "global_var.h"
#include "module_var.h"
#include "file.h"
#include "utils.h"
#include "input_output.h"
#include "log.h"

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Filepath to the original configuration files.
#define BSD_RSYSLOG_CONFIG_ORIGINAL     "/usr/local/etc/rsyslog.conf"
#define BSD_RSYSLOG_REMOTE_ORIGINAL     "/usr/local/etc/rsyslog.d/50-default.conf"

// Filepath to the configuration files to be used/edited in UHB.
#define BSD_RSYSLOG_CONFIG_UHB          "/root/uhb/base/config/current/rsyslog.conf"
#define BSD_RSYSLOG_REMOTE_UHB          "/root/uhb/base/config/current/50-default.conf"

// Filepath to the backup of all configuration files.
#define BSD_RSYSLOG_CONFIG_BACKUP       "/root/uhb/base/config/backups/rsyslog.conf"
#define BSD_RSYSLOG_REMOTE_BACKUP       "/root/uhb/base/config/backups/50-default.conf"

#define MAX_PORT_SIZE 6 // Buffer up to 6 characters 5 digits + '\0'

static bool rfc_5424_send;
static bool rfc_5424_write;

char udp_port[MAX_PORT_SIZE] = "514";   // Port for the UDP listening module - port 514 as startup default
char tcp_port[MAX_PORT_SIZE] = "514";   // Port for the TCP listening module - port 514 as startup default

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
    if(system("service rsyslogd status >/dev/null 2>&1") == 0){
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
    snprintf(command, sizeof(command), "service rsyslogd restart");
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
    if(find_string_in_file(SEND_RFC5424,BSD_RSYSLOG_CONFIG_UHB)){
        rfc_5424_send = true;
    }else{
        rfc_5424_send = false;
    }
    if(find_string_in_file(WRITE_RFC5424,BSD_RSYSLOG_CONFIG_UHB)){
        rfc_5424_write = true;
    }else{
        rfc_5424_write = false;
    }
}

void apply_rfc5424() {
    int opt = get_yes_no_input("MSG 1/2: Use RFC5424 standard for remote logs? (Y/N):");
    if(opt == 0 && !find_string_in_file(SEND_RFC5424,BSD_RSYSLOG_CONFIG_UHB)){
        append_to_file(SEND_RFC5424,BSD_RSYSLOG_CONFIG_UHB);
        rfc_5424_send = true;
    }else if(opt == 1){
        find_and_replace(SEND_RFC5424,"",BSD_RSYSLOG_CONFIG_UHB);
        rfc_5424_send = false;
    }
    opt = get_yes_no_input("MSG 2/2: Use RFC5424 standard when storing logs in the system? (Y/N):");
    if(opt == 0 && !find_string_in_file(WRITE_RFC5424,BSD_RSYSLOG_CONFIG_UHB)){
        append_to_file(WRITE_RFC5424,BSD_RSYSLOG_CONFIG_UHB);
        rfc_5424_write = true;
    }else if(opt == 1){
        find_and_replace(WRITE_RFC5424,"",BSD_RSYSLOG_CONFIG_UHB);
        rfc_5424_write = false;
    }
}

void initialize_logging(bool copy_from_backup){
    if(log_exists() && check_logging_status()){
        if(!copy_from_backup){
            // Copy original conf to UHB
            copy_file(BSD_RSYSLOG_CONFIG_ORIGINAL,BSD_RSYSLOG_CONFIG_UHB);
            copy_file(BSD_RSYSLOG_REMOTE_ORIGINAL,BSD_RSYSLOG_REMOTE_UHB);
            // Copy original conf to backup
            copy_file(BSD_RSYSLOG_CONFIG_ORIGINAL,BSD_RSYSLOG_CONFIG_BACKUP);
            copy_file(BSD_RSYSLOG_REMOTE_ORIGINAL,BSD_RSYSLOG_REMOTE_BACKUP);
        }else{
            // Copy backup to UHB
            copy_file(BSD_RSYSLOG_CONFIG_BACKUP,BSD_RSYSLOG_CONFIG_UHB);
            copy_file(BSD_RSYSLOG_REMOTE_BACKUP,BSD_RSYSLOG_REMOTE_UHB);
        }
        detect_rfc5424();
    }else if(log_exists() && !check_logging_status()){
        printf("WRN: Rsyslog daemon is detected, but not running!\n");
    }
}

void reset_logging_configuration() {
    printf("MSG: Resetting auditing configuration...\n");
    // Copy backup to UHB
    copy_file(BSD_RSYSLOG_CONFIG_BACKUP,BSD_RSYSLOG_CONFIG_UHB);
    copy_file(BSD_RSYSLOG_REMOTE_BACKUP,BSD_RSYSLOG_REMOTE_UHB);
}

void view_logging_configuration() {
    int opt = three_option_input("MSG 1/2: View rsyslog.conf file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(BSD_RSYSLOG_CONFIG_UHB);
    if(opt == 2)
         return;
    opt = three_option_input("MSG 2/2: View 50-default.conf file?. (Y)es/(N)o/E(x)it:",'Y','N','X');
    if(opt == 0)
        view_file(BSD_RSYSLOG_REMOTE_UHB);
    if(opt == 2)
         return;
}

bool apply_logging_configuration() {
    printf("MSG: Applying UHB logging configuration...\n");
    if(copy_file(BSD_RSYSLOG_CONFIG_UHB,BSD_RSYSLOG_CONFIG_ORIGINAL)){
        if(restart_logging_daemon()){
            printf("MSG: UHB logging configuration successfully applied.\n");
            return true;
        }else{
            fprintf(stderr, "ERR: apply_logging_configuration(): Could not restart logging daemon.\n");
            return false;
        }
    }else{
        fprintf(stderr, "ERR: apply_logging_configuration(): Could not apply configuration file.\n");
        return false;
    }
}

// View logging configuration is already implemented in the menu function

/**
 * Functions to manage logging inside the system
 */

void add_local_logging() {
    char msg[MAX_LINE_LENGTH];
    char fp[MAX_LINE_LENGTH];
    char command[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG 1/2: Please enter the messages you want to log:",msg,sizeof(msg));
        opt = get_yes_no_input("MSG: Is the information correct? (Y/N):");
    }
    opt = 1;
    while(opt == 1){
        get_user_input("MSG 2/2: Please enter where these log messages will be stored:",fp,sizeof(fp));
        opt = get_yes_no_input("MSG: Is the information correct? (Y/N):");
    }
    snprintf(command,sizeof(command),"%s    %s\n",msg, fp);
    append_to_file(command,BSD_RSYSLOG_CONFIG_UHB);
    if(!path_exists(fp)){
        return create_file(fp);
    }else{
        return true;
    }
}

// Add logrotate functionality afterwards

/**
 * Functions for remote logging/log forwarding
 */

void enable_udp_module(){
    char a[MAX_LINE_LENGTH];
    snprintf(a,sizeof(a),"input(type=\"imudp\" port=\"%s\")",udp_port);
    find_and_replace("#module(load=\"imudp\")","module(load=\"imudp\")",BSD_RSYSLOG_CONFIG_UHB);
    find_and_replace("#input(type=\"imudp\" port=\"514\")",a,BSD_RSYSLOG_CONFIG_UHB);
}

void enable_tcp_module(){
    char b[MAX_LINE_LENGTH];
    snprintf(b,sizeof(b),"input(type=\"imtcp\" port=\"%s\")",tcp_port);
    find_and_replace("#module(load=\"imtcp\")","module(load=\"imtcp\")",BSD_RSYSLOG_CONFIG_UHB);
    find_and_replace("#input(type=\"imtcp\" port=\"514\")",b,BSD_RSYSLOG_CONFIG_UHB);
}

void disable_udp_module(){
    char a[MAX_LINE_LENGTH];
    snprintf(a,sizeof(a),"input(type=\"imudp\" port=\"%s\")",udp_port);
    find_and_replace("module(load=\"imudp\")","#module(load=\"imudp\")",BSD_RSYSLOG_CONFIG_UHB);
    find_and_replace(a,"#input(type=\"imudp\" port=\"514\")",BSD_RSYSLOG_CONFIG_UHB);
}

void disable_tcp_module(){
    char b[MAX_LINE_LENGTH];
    snprintf(b,sizeof(b),"input(type=\"imtcp\" port=\"%s\")",tcp_port);
    find_and_replace("module(load=\"imtcp\")","#module(load=\"imtcp\")",BSD_RSYSLOG_CONFIG_UHB);
    find_and_replace(b,"#input(type=\"imtcp\" port=\"514\")",BSD_RSYSLOG_CONFIG_UHB);
}

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
    find_and_replace("#module(load=\"imudp\")","module(load=\"imudp\")",BSD_RSYSLOG_CONFIG_UHB);
    return find_and_replace(current,replace,BSD_RSYSLOG_CONFIG_UHB);
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
    find_and_replace("#module(load=\"imtcp\")","module(load=\"imtcp\")",BSD_RSYSLOG_CONFIG_UHB);
    return find_and_replace(current,replace,BSD_RSYSLOG_CONFIG_UHB);
}

void set_log_reception_service(){
    char port[MAX_PORT_SIZE];
    int opt = get_yes_no_input("MSG 1/2: Would you like to listen for receiving UDP log messages? (Y/N):");
    if(opt == 0){
        enable_udp_module();
        get_user_input("MSG: Which port would you like to use?:",port,sizeof(port));
        edit_udp_module(port);
        strcpy(udp_port,port);
    }else if(opt == 1){
        disable_udp_module();
    }
    opt = get_yes_no_input("MSG 2/2: Would you like to listen for receving TCP log messages? (Y/N):");
    if(opt == 0){
        enable_tcp_module();
        get_user_input("MSG: Which port would you like to use?:",port,sizeof(port));
        edit_tcp_module(port);
        strcpy(tcp_port,port);
    }else if(opt == 1){
        disable_tcp_module();
    }
}

void add_log_reception_rule(){
    char filename[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    get_user_input("MSG: Where would you like to store receving log messages?\n",filename,sizeof(filename));
    snprintf(line,sizeof(line),"\n$template RemoteLogs,\"%s\"",filename);
    append_to_file(line,BSD_RSYSLOG_CONFIG_UHB);
    append_to_file("*.* ?RemoteLogs",BSD_RSYSLOG_CONFIG_UHB);
    append_to_file("& ~",BSD_RSYSLOG_CONFIG_UHB);
}

void add_log_forwarding_rule(){
    char command[MAX_LINE_LENGTH];
    char ip[20];
    char port[MAX_PORT_SIZE];
    char directive[MAX_LINE_LENGTH];
    get_user_input("MSG 1/4: Please enter the remote server IP address:",ip,sizeof(ip));
    get_user_input("MSG 2/4: Please enter the port number to be used:",port,sizeof(port));
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG 3/4: Please add additional rsyslog file and filter directives:",directive,sizeof(directive));
        opt = get_yes_no_input("MSG: Are the added directives correct? (Y/N):");
    }
    opt = three_option_input("MSG 4/4: Is the remote server using (U)DP or (T)CP? X to E(x)it:",'U','T','X');
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
    append_to_file(command,BSD_RSYSLOG_REMOTE_ORIGINAL);
}

void view_logging_manual() {
    system("man rsyslogd");
    system("clear");
}