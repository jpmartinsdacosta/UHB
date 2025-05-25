#include <stdio.h>
#include <stdbool.h>

#include "global_var.h"
#include "module_var.h"
#include "file.h"
#include "utils.h"
#include "input_output.h"
#include "log.h"

#define RSYSLOG_ORIGINAL_CONF "/usr/local/etc/rsyslog.conf"
#define RSYSLOG_BACKUP_CONF "/root/uhb/base/config/template/rsyslog.conf.template"

#define SEND_RFC5424 "$ActionForwardDefaultTemplate RSYSLOG_SyslogProtocol23Format"
#define WRITE_RFC5424 "$ActionFileDefaultTemplate RSYSLOG_SyslogProtocol23Format"

static bool rfc_5424;

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

void check_log_config() {
    if(path_exists(RSYSLOG_ORIGINAL_CONF)){

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

