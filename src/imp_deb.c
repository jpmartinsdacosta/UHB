#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "imp_deb.h"
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__linux__)
// Debian headers.
#endif

int imp_deb() {
    printf("DEB implementation\n");
    return 1;
}