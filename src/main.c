#include <stdio.h>

int main() {
    int os = os_detect();
    // Check if UHB is being executed by the root user.
    // TODO: Check if UHB is being executed by the uhb user, using the getpwnam function.
    if (getuid() != 0) {
        printf("ERR: UHB must be executed by the root user.\n");
        return -1;
    }
    
    // Provide the user with a menu to choose the desired action.
    if(os != -1){
        exec_exists_common();
        config_exists();    // Check if the configuration file exists.
        main_menu();        // Show the menu to the user.
    }else{
        return -1;
    }
    return 0;
}
