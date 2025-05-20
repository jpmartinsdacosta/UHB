#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "os_interface.h"
#include "global_var.h"

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__FreeBSD__)
// FreeBSD headers.
#endif

char os[] = "## uhb_os = BSD";

char* get_os(){
    return os;
}


