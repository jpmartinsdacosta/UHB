#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Custom headers.
#include "so_detect.h"
#include "utils.h"

// OS-specific headers, including the ones needed for VS Code.
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#else
// Headers needed for FreeBSD/Debian in main.c.
#endif


int main() {
    int os = 0, menu = -1;

    // Check if UHB is being executed by the root user.
    // TODO: Check if UHB is being executed by the uhb user, using the getpwnam function.
    if (getuid() != 0) {
        printf("UHB must be executed by the root user\n");
        return -1;
    }

    // Detect the operating system and execute its respective implementation.
    switch (os = so_detect()) {
        case -1:
            printf("Failed to detect or unsupported OS.\n");
            return 1;
        case 0:
            printf("FreeBSD OS detected.\n");
            break;
        case 1:
            printf("Debian OS detected.\n");
            break;
    }

    // Provide the user with a menu to choose the desired action.
    while (menu == -1)
    {
        // TODO: Show current changes to the user every time the menu is displayed.
        printf("Choose an option:\n");
        printf("1. Check if file exists.\n");
        printf("0. Exit.\n");
        scanf("%d", &menu);
        switch (menu)
        {
            case 1: 
                path_test();
                menu = -1;
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option.\n");
                menu = -1;
                break;
        }
    }

    return 0;
}
