#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "imp_bsd.h"
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__FreeBSD__)
// FreeBSD headers.
#endif

int imp_bsd() {
    printf("BSD implementation\n");
    return 1;
}