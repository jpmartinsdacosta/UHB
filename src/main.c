#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "so_detect.h"
#include "imp_bsd.h"
#include "imp_deb.h"

int main() {
    int os = 0;
    int deb, bsd;
    // Check if UHB is being executed by the root user.
    // TODO: Check if UHB is being executed by the uhb user, using the getpwnam function.
    if(getuid() != 0) {
        printf("UHB must be executed by the root user\n");
        return -1;
    }

    // Detect the operating system and execute its respective implementation.
     
    switch (os = so_detect()) {
        case -1:
            printf("Failed to detect or unknown operating system\n");
            return 1;
        case 0:
            printf("FreeBSD\n");
            bsd = imp_bsd();
            break;
        case 1:
            printf("Debian\n");
            deb = imp_deb();
            break;
    }
    if (bsd == 1 || deb == 1) {
        printf("Success\n");
    }
    return 0;
}
