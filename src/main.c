#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Custom headers.
#include "utils.h"

// OS-specific headers, including the ones needed for VS Code.
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#else
// Headers needed for FreeBSD/Debian in main.c.
#endif


int main() {
    // Check if UHB is being executed by the root user.
    // TODO: Check if UHB is being executed by the uhb user, using the getpwnam function.
    if (getuid() != 0) {
        printf("UHB must be executed by the root user\n");
        return -1;
    }
    int os = so_detect();

    // Provide the user with a menu to choose the desired action.
    if(os != -1){
        show_menu(os);
    }else{
        printf("Failed to detect or unsupported OS.\n");
        return -1;
    }
    return 0;
}
