#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "config.h"

int main() {
    // Check if UHB is being executed by the root user.
    // TODO: Check if UHB is being executed by the uhb user, using the getpwnam function.
    if (getuid() != 0) {
        printf("ERR: UHB must be executed by the root user\n");
        return -1;
    }
    int os = so_detect();

    // Provide the user with a menu to choose the desired action.
    if(os != -1){
        config_exists();    // Check if the configuration file exists.
        show_menu();        // Show the menu to the user.
    }else{
        return -1;
    }
    return 0;
}
