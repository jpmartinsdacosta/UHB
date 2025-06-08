#include <stdio.h>
#include <stdbool.h>

#include "file.h"
#include "config.h"
#include "utils.h"
#include "input_output.h"
#include "global_var.h"
#include "policy.h"
#include "mac.h"

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Filepath to the original configuration files.
#define BSD_MAC_CONFIG_ORIGINAL "/etc/rc.bsdextended"

// Filepath to the configuration files to be used/edited in UHB.
#define BSD_MAC_CONFIG_CURRENT  "/root/uhb/base/config/current/rc.bsdextended"

// Filepath to the backup of all configuration files.
#define BSD_MAC_CONFIG_BACKUP   "/root/uhb/base/config/backups/rc.bsdextended"

#define MAC_MODE_ADMIN  0x01
#define MAC_MODE_READ   0x02
#define MAC_MODE_WRITE  0x04
#define MAC_MODE_EXEC   0x08
#define MAC_MODE_ATTR   0x10  // 's' access to file attributes
#define MAC_MODE_NONE   0x00

/**
 * MAC functions available to the user in the menu
 */

bool mac_exists() {
    if(system("kldstat | grep mac_bsdextended > /dev/null 2>&1") == 0){
        return true;
    }else{
        printf("MSG: MAC was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

bool get_mac() {
    system("ugidfw list | less");
    system("clear");
}

int parse_mac_mode_flags(const char *mode_str) {
    int flags = 0;
    if (!mode_str) return 0;

    for (size_t i = 0; i < strlen(mode_str); i++) {
        switch (mode_str[i]) {
            case 'a': flags |= MAC_MODE_ADMIN; break;
            case 'r': flags |= MAC_MODE_READ; break;
            case 'w': flags |= MAC_MODE_WRITE; break;
            case 'x': flags |= MAC_MODE_EXEC; break;
            case 's': flags |= MAC_MODE_ATTR; break;
            case 'n': flags = MAC_MODE_NONE; return flags; // 'none' overrides all
            default: break; // ignore unknown chars
        }
    }
    return flags;
}

bool set_mac() {
    char input[MAX_LINE_LENGTH];
    char subject[MAX_NAME_LENGTH];
    char uid[MAX_NAME_LENGTH];
    char gid[MAX_NAME_LENGTH];
    char object[MAX_NAME_LENGTH];
    char filesys[MAX_LINE_LENGTH];
    char type[MAX_LINE_LENGTH];
    char mode[MAX_LINE_LENGTH];
    int opt = 1;
    while(opt == 1){
        get_user_input("MSG: Please insert ugidfw rule to be added:\n",input,sizeof(input));
        parse_input_next_token(input," ","subject",subject,sizeof(subject));
        parse_input_next_token(input," ","uid",uid,sizeof(uid));
        parse_input_next_token(input," ","gid",gid,sizeof(gid));
        parse_input_next_token(input," ","object",object,sizeof(object));
        parse_input_next_token(input," ","filesys",filesys,sizeof(filesys));
        parse_input_next_token(input," ","type",type,sizeof(type));
        parse_input_next_token(input," ","mode",mode,sizeof(mode));
        opt = three_option_input("MSG: Is the above information correct? (Y)es/(N)o/E(x)it:",'Y','N','X');
    }
    if(opt == 0){
        append_to_file(input, BSD_MAC_CONFIG_CURRENT);
        add_mac_element(filesys,input,subject,uid,gid,object,type,mode);
        return true;
    }else{
        fprintf(stderr,"ERR: set_mac(): Failed to add MAC policy entry.\n");
        return false;
    }
}

void rem_mac_rule() {
    remove_last_n_lines(BSD_MAC_CONFIG_CURRENT,2);
}

void initialize_mac_module(bool copy_from_backup) {
    if(mac_exists()){
        if(!copy_from_backup){
            copy_file(BSD_MAC_CONFIG_ORIGINAL,BSD_MAC_CONFIG_CURRENT);
            copy_file(BSD_MAC_CONFIG_ORIGINAL,BSD_MAC_CONFIG_BACKUP);
        }else{
            copy_file(BSD_MAC_CONFIG_BACKUP,BSD_MAC_CONFIG_CURRENT);
        }
    }
}

void view_mac_manual() {
    system("man ugidfw");
    system("clear");
}

void view_mac_configuration() {
    view_file(BSD_MAC_CONFIG_CURRENT);
}

void reset_mac_configuration(bool load_from_backup) {
    if(!load_from_backup){
        copy_file(BSD_MAC_CONFIG_ORIGINAL,BSD_MAC_CONFIG_CURRENT);
        copy_file(BSD_MAC_CONFIG_ORIGINAL,BSD_MAC_CONFIG_BACKUP);
    }else{
        copy_file(BSD_MAC_CONFIG_BACKUP,BSD_MAC_CONFIG_CURRENT);
    }
}

void apply_mac_configuration() {
    copy_file(BSD_MAC_CONFIG_CURRENT,BSD_MAC_CONFIG_ORIGINAL);
    printf("MSG: To apply MAC configuration in FreeBSD, reboot the system!\n");
}

void free_mac() {
    //free_fc(&mode_flags);
    //free_fc(&type_flags);
}