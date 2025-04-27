#include <stdio.h>
#include "perms.h"

int main() {
    int os = os_detect();
    // TODO: Check if UHB is being executed by the uhb_array user, using the getpwnam function.
    if (getuid() != 0) {
        printf("ERR: UHB must be executed by the root user.\n");
        return -1;
    }
    if(os != -1){
        init_uhb_array();       // Initialize array of permission structures.
        exec_exists_common();
        config_exists();        // Check if the configuration file exists.
        main_menu();            // Show the menu to the user.
        clear_uhb_array();      // Always clear array of permission structures when done.
    }else{
        return -1;
    }
    return 0;
}
