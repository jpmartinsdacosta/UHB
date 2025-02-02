#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int so_detect(){
    FILE *fp;
    char path[1035];
    char *os;
    int os_id = -1;

    // Open the command for reading.
    fp = popen("uname -s", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        return -1;
    }

    // Read the output a line at a time - output it.
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        os = strtok(path, "\n");
        if(strcmp(os, "FreeBSD") == 0){
            os_id = 0;
        } else if(strcmp(os, "Linux") == 0){
            os_id = 1;
        }
    }

    // Close the command.
    pclose(fp);
    return os_id;
}
