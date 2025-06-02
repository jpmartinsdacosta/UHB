#include <stdio.h>
#include <stdbool.h>
#include "global_var.h"
#include "utils.h"

// File naming convention: <OS>_<MODULE>_<FILENAME>_<FILEPATH>

// Filepath to the original configuration files.


// Filepath to the configuration files to be used/edited in UHB.


// Filepath to the backup of all configuration files.


bool fwl_exists() {
    if(exec_exists("ufw")){
        return true;
    }else{
        printf("MSG: Firewall was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

bool check_firewall_status() {
    if(system("service ufw status >/dev/null 2>&1") == 0){
        return true;
    }else{
        return false;
    }
}

void view_firewall_configuration(){

}

void view_firewall_manual(){
    system("man ufw");
    system("clear");
}