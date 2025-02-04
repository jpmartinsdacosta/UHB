#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <so_detect.h>
#include <imp_bsd.h>
#include <imp_deb.h>

int main() {
    int os = 0;
    bool eop = false;
    
    // Check if UHB is being executed by the root user.
    // TODO: Check if UHB is being executed by the uhb user, using the getpwnam function.
    if(getuid() != 0) {
        printf("UHB must be executed by the root user\n");
        return -1;
    }

    // Detect the operating system and return its name.
    if((os = so_detect()) == -1) {
        printf("Failed to detect the operating system\n");
        return 1;
    }else if(os == 0){
        printf("FreeBSD\n");
    }else if(os == 1){
        printf("Debian\n");
    }
    return 0;
}
