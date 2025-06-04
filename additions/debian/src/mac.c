#include <stdio.h>
#include <stdbool.h>

#include "file.h"
#include "config.h"
#include "utils.h"
#include "input_output.h"
#include "global_var.h"
#include "policy.h"


#define BSD_MAC_CONFIG_ORIGINAL "/etc/rc.bsdextended"
#define BSD_MAC_CONFIG_CURRENT  "../config/current/rc.bsdextended"
#define BSD_MAC_CONFIG_BACKUP   "../config/backups/rc.bsdextended"

bool mac_exists() {
    printf("MSG: MAC was NOT detected. Configuration will NOT be applied.\n");
    return false; 
}

void get_mac() {
    system("ugidfw list | less ; clear");
}


void set_mac() {
    char rule[MAX_LINE_LENGTH];
    char uid[MAX_NAME_LENGTH];
    char gid[MAX_NAME_LENGTH];
    char path[MAX_LINE_LENGTH];
    char type[MAX_LINE_LENGTH];
    char mode[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG: Please insert the MAC rule you wish to add:",rule,sizeof(rule));
        opt = three_option_input("MSG: Is the provided information correct? (Y)es/(N)o/E(x)it:",'Y','N','X');
    }
    if(opt == 0){
        parse_input_next_token(rule," ","uid",uid,sizeof(uid));
        parse_input_next_token(rule," ","gid",gid,sizeof(gid));
        parse_input_next_token(rule," ","type",type,sizeof(type));
        parse_input_next_token(rule," ","mode",mode,sizeof(mode));
    }
}

void view_mac_manual() {
    system("man ugidfw");
}

void view_mac_configuration() {
    view_file(BSD_MAC_CONFIG_CURRENT);
}

void reset_mac_configuration() {

}

void apply_mac_configuration() {
    copy_file(BSD_MAC_CONFIG_CURRENT,BSD_MAC_CONFIG_ORIGINAL);
    printf("MSG: To apply MAC configuration in FreeBSD, reboot the system!\n");
}
