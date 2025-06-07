#include <stdio.h>
#include <stdbool.h>

#include "file.h"
#include "config.h"
#include "utils.h"
#include "input_output.h"
#include "global_var.h"
#include "policy.h"

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Filepath to the original configuration files.
#define BSD_MAC_CONFIG_ORIGINAL "/etc/rc.bsdextended"

// Filepath to the configuration files to be used/edited in UHB.
#define BSD_MAC_CONFIG_CURRENT  "/root/uhb/base/config/current/rc.bsdextended"

// Filepath to the backup of all configuration files.
#define BSD_MAC_CONFIG_BACKUP   "/root/uhb/base/config/backups/rc.bsdextended"

FlagCollection type_fc, mode_fc;

static const char type_flags[] = {
    'a',    // Any file type
    'r',    // A regular file
    'd',    // A directory
    'b',    // A block special device
    'c',    // A character special device
    'l',    // A symbolic link
    's',    // A unix domain socket
    'p'     // A named pipe (FIFO)
};

static const char mode_flags[] = {
    'a',    // Administrative operations
    'r',    // Read access
    's',    // Access to file attributes
    'w',    // Write access
    'x',    // Execute access
    'n'     // None
};

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

void set_mac() {
    char input[MAX_LINE_LENGTH];
    char subject[MAX_NAME_LENGTH];
    char uid[MAX_NAME_LENGTH];
    char gid[MAX_NAME_LENGTH];
    char object[MAX_NAME_LENGTH];
    char filesys[MAX_LINE_LENGTH];
    char type[MAX_LINE_LENGTH];
    char mode[MAX_LINE_LENGTH];
    int opt = 1;
    //init_flag(&type_fc,sizeof(type_flags)-1,type_flags);
    //init_flag(&mode_fc,sizeof(mode_flags)-1,mode_flags);
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
    }else{
        fprintf(stderr,"ERR: set_mac(): Failed to add MAC policy entry.\n");
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
    view_mac_manual();
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
