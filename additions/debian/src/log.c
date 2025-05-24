#include <stdio.h>
#include <stdbool.h>

#include "global_var.h"
#include "module_var.h"
#include "file.h"
#include "utils.h"
#include "input_output.h"

#define RSYSLOG_ORIGINAL_CONF "/etc/rsyslog.conf"

#define SEND_RFC5424 "$ActionForwardDefaultTemplate RSYSLOG_SyslogProtocol23Format"
#define WRITE_RFC5424 "$ActionFileDefaultTemplate RSYSLOG_SyslogProtocol23Format"

static bool rfc_5424;

bool rsyslog_exists() {
    if(exec_exists("rsyslog")){
        printf("MSG: Logging daemon was detected.\n");
        return true;
    }else{
        printf("MSG: Logging daemon was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

bool check_rsyslog_status(){
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

bool detect_rfc5424(){
    if(find_string_in_file(SEND_RFC5424,RSYSLOG_ORIGINAL_CONF) && find_string_in_file(WRITE_RFC5424,RSYSLOG_ORIGINAL_CONF)){
        printf("MSG: RFC5424 standard detected.\n");
        return true;
    }else{
        printf("MSG: RFC3164 standard detected.\n");
        return false;
    }
}

void initialize_rsyslog(){
    if(rsyslog_exists() && check_rsyslog_status()){
        rfc_5424 = detect_rfc5424;
        copy_file(RSYSLOG_ORIGINAL_CONF,"/root/uhb/base/config/rsyslog.conf");
    }else if(rsyslog_exists() && !check_rsyslog_status()){
        printf("WRN: Rsyslog daemon is detected, but not running!\n");
    }
}

bool restart_rsyslog(){
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

bool set_local_logging(){
   
}

int set_remote_logging(){
    int opt = three_option_input("OPT: Which protocol is to be used? (U)DP or (T)CP? (X) to cancel.\n",'u','t','x');
    if(opt == 0){
        //append_to_file("module(load=\"imudp\")",);
        //append_to_file("input(type=\"imudp\" port=\"514\")",);
        printf("MSG: Rsyslogd listening for UDP at port 514.\n");
    }else if(opt == 1){
        //append_to_file("module(load=\"imtcp\")",);
        //append_to_file("input(type=\"imtcp\" port=\"514\")",);
        printf("MSG: Rsyslogd listening for TCP at port 514.\n");
    }else if(opt == 2){
        return 0;
    }

}

