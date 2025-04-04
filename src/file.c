#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_MAX 256
#define MAX_CMD 300

int find_first_in_file(char *target, char *filepath){
    char line[LINE_MAX];
    int position = 0;
    bool found = false;
    if(!path_exists(filepath)){
        printf("ERR: The selected file does not exist.\n");
        return -1;
    }else{
        FILE *file = fopen(filepath, "r");
        if (file) {
            while (fgets(line, LINE_MAX, file) && !found) {
                if (strstr(line, target)) {
                    found = true;
                }else{
                    position++;
                }                    
            }
            fclose(file);
            return found ? position : -1;
        }else{
            printf("ERR: Error reading %s file.\n",filepath);
            return -1;
        }
    }
}

bool find_strings_in_line(char *a, char *b, char *filepath) {
    char line[LINE_MAX];
    int position = 0;

    if (!path_exists(filepath)) {
        printf("ERR: The selected file does not exist.\n");
        return false;
    } else {
        FILE *file = fopen(filepath, "r");
        if (file) {
            while (fgets(line, LINE_MAX, file)) {
                if (strstr(line, a) && strstr(line, b)) {
                    printf("MSG: Both strings found on line %d\n", position);
                    fclose(file);
                    return true;
                }
                position++;
            }
            fclose(file);
            return false;
        } else {
            printf("ERR: Error reading %s file.\n",filepath);
            return false;
        }
    }
}

char* find_partition_from_file(char *filepath){
    FILE* pipe;
    char command[MAX_CMD];
    char filesystem[LINE_MAX];
    snprintf(command,sizeof(command),"df \"%s\" | tail -1 | awk '{print $1}'",filepath);
    pipe = popen(command, "r");
    if(pipe == NULL){
        perror("popen");
        return NULL;
    }
    if (fgets(filesystem, sizeof(filesystem), pipe) != NULL) {
        filesystem[strcspn(filesystem, "\n")] = 0;
    }
    pclose(pipe);
    return strdup(filesystem);
}

void test_function(){
   find_strings_in_line("/anony","tmpfs","../tests/test.txt");
}

