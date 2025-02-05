#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__FreeBSD__)
#include <unistd.h>
#include <sys/utsname.h>
#include "imp_bsd.h"
#elif defined (__linux__)
#include <unistd.h>
#include <sys/utsname.h>
#include "imp_deb.h"
#endif

bool path_exists(char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

void get_dac_common() {
    char path[100];
    char command[150];
    FILE *file;
    printf("Please enter the path to test: ");
    scanf("%s", path);
    if (path_exists(path)) {
        snprintf(command, sizeof(command), "ls -l %s", path);
        file = popen(command, "r");
        if (file) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                printf("%s", buffer);
            }
            pclose(file);
        } else {
            printf("Failed to run command.\n");
        }
    } else {
        printf("Path does not exist.\n");
    }
}



