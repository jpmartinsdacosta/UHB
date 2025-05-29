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
#elif defined (__FreeBSD__)
// FreeBSD headers.
#endif



char os[] = "## uhb_os = BSD";

char* get_os(){
    return os;
}

bool enabled_in_rc_conf(const char *service){
    char command[MAX_LINE_LENGTH];
    snprintf(command,sizeof(command),"%s=\"YES\"",service);
    return find_string_in_file(command,RC_CONF);
}
