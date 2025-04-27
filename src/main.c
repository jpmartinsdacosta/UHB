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
        init_dac_array();       // Initialize DACStruct 
        exec_exists_common();
        config_exists();        // Check if the configuration file exists.
        main_menu();            // Show the menu to the user.
        clear_dac_array();      // Clear DACStruct
    }else{
        return -1;
    }
    return 0;
}
