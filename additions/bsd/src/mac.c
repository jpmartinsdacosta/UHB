#include <stdio.h>
#include <stdbool.h>

#include "file.h"
#include "config.h"
#include "utils.h"
#include "input_output.h"
#include "global_var.h"
#include "policy.h"

FlagCollection object_fc, mode_fc;

const char object_flags[] = {
    'a',    // Any file type
    'r',    // A regular file
    'd',    // A directory
    'b',    // A block special device
    'c',    // A character special device
    'l',    // A symbolic link
    's',    // A unix domain socket
    'p'     // A named pipe (FIFO)
};

const char mode_flags[] = {
    'a',    // Administrative operations
    'r',    // Read access
    's',    // Access to file attributes
    'w',    // Write access
    'x',    // Execute access
    'n'     // None
};


bool mac_exists() {
    if(system("kldstat | grep mac_bsdextended > /dev/null 2>&1") == 0){
        return true;
    }else{
        printf("MSG: ACL was NOT detected. Configuration will NOT be applied.\n");
        return false;
    }
}

bool get_mac() {

}

bool set_mac() {
    
}