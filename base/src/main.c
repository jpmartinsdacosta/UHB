#include <stdio.h>
#include "utils.h"
#include "policy.h"
#include "menu.h"
#include "config.h"
#include "log.h"

int main() {
    int os = os_detect();
    // TODO: Check if UHB is being executed by the uhb_array user, using the getpwnam function.
    if (getuid() != 0) {
        printf("ERR: UHB must be executed by the root user.\n");
        return -1;
    }
    if(os != -1){
        init_dac_array();
        detect_execs();
        uhb_conf_exists(CONFIG_UHB);        // Check if the configuration file exists.
        initialize_logging();
        main_menu();                        // Show the menu to the user.
        clear_dac_array();
    }else{
        return -1;
    }
    return 0;
}
