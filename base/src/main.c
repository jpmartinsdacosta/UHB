#include <stdio.h>

#include "menu.h"
#include "config.h"


int main() {
    int os = os_detect();
    // TODO: Check if UHB is being executed by the uhb_array user, using the getpwnam function.
    if (getuid() != 0) {
        printf("ERR: UHB must be executed by the root user.\n");
        return -1;
    }
    if(os != -1){
        initialize_uhb();
        main_menu();
        terminate_uhb();
    }else{
        return -1;
    }
    return 0;
}
