#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "file.h"
#include "os_interface.h"
#include "module_var.h"
#include "global_var.h"

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#elif defined (__linux__)
// Debian headers.
#endif

char os[] = "## uhb_os = DEB";

char* get_os(){
    return os;
}
