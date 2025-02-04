#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "so_detect.h"
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/utsname.h>
#endif

int so_detect() {
    #ifdef _WIN32
    return -1;
    #else
    struct utsname uts;
    if (uname(&uts) == -1) {
        return -1;
    }
    if (strcmp(uts.sysname, "FreeBSD") == 0) {
        return 0;
    } else if (strcmp(uts.sysname, "Linux") == 0) {
        return 1;
    }
    return -1;
    #endif
}
