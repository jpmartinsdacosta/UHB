#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

bool exec[4] = {false, false, false, false};    // Array of detected executables.

const char *main_menu_options[] = {             // Constant array of main menu options.
    "1. Get/Set DAC on a file/directory",
    "2. Get/Set ACLs on a file/directory",
    "3. Firewall configuration",
    "4. Logging configuration",
    "5. Auditing configuration",
    "6. MAC configuration",
    "7. Configuration file options",
    "0. Exit UHB",
    NULL
};

const char *dac_menu_options[] = {
    "1. Get DAC of a file/directory",
    "2. Set DAC of a file/directory",
    "0. Return to Main Menu",
    NULL
};

const char *conf_menu_options[] = {
    "1. View configuration file",
    "2. Clear configuration file",
    "3. Apply changes from configuration file",
    "0. Return to Main Menu",
    NULL
};

void exec_exists_common (){
    printf("INI: Detected supported programs:\n"); // Change this printf's place
    #ifdef __FreeBSD__
        exec_exists_bsd(exec);
    #elif defined (__linux__)
        exec_exists_deb(exec);
    #endif  
}

int get_menu_size(const char **options){
    int size = 0;
    while(options[size] != NULL){
        size++;
    }
    return size;
}

void final_prompt(){
    int choice = -1;
    char input[3];
    while(choice == -1 && is_config_modified()){
        choice = get_yes_no_input("MSG: Clear the config file before leaving? (y/n):\n");
        if(choice == 0){
            set_initial_config();
            printf("MSG: Config file cleared, exiting UHB...\n");
        }else{
            printf("MSG: Exiting UHB...\n");
        }
    }
}

int display_menu(char *prompt, const char **options){
    int choice = -1;
    char input[3];
    printf("%s\n", prompt);
    for(int i = 0; i < get_menu_size(options); i++){
        printf("%s\n", options[i]);
    }
    if(get_user_input("MSG: Please select an option:", input, sizeof(input)) != -1){
        choice = atoi(input);
        system("clear");
        return choice;
    }else
        system("clear");
        return -1;
}

void dac_menu(){
    int choice = -1;
    while(choice != 0){
        choice = display_menu("DAC Menu", dac_menu_options);
        switch(choice){
            case 1:
                get_dac_common();
                break;
            case 2:
                set_dac_common();
                break;
            case 0:
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}

void conf_menu(){
    int choice = -1;
    while(choice != 0){
        choice = display_menu("Configuration File Menu", conf_menu_options);
        switch(choice){
            case 1:
                view_config();
                break;
            case 2:
                set_initial_config();
                break;
            case 3:
                apply_config(os_detect());
                break;
            case 0:
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}

void main_menu(){
    int choice = -1;
    while(choice != 0){
        choice = display_menu("UHB Main Menu", main_menu_options);
        switch(choice){
            case 1:
                dac_menu();
                break;
            case 2:
                exec[0] ? printf("MSG: Option not implemented yet.\n") : printf("MSG: Option not implemented yet.\n");
                break;
            case 3:
                exec[1] ? printf("MSG: Option not implemented yet.\n") : printf("MSG: Option not implemented yet.\n");
                break;
            case 4:
                exec[2] ? printf("MSG: Option not implemented yet.\n") : printf("MSG: Option not implemented yet.\n");
                break;
            case 5:
                exec[3] ? printf("MSG: Option not implemented yet.\n") : printf("MSG: Option not implemented yet.\n");
                break;
            case 6:
                printf("MSG: Option not implemented yet.\n");
                break;
            case 7:
                conf_menu();
                break;
            case 0:
                final_prompt();
                printf("\nFIN: Goodbye!\n");
                break;
            default:
                printf("ERR: Invalid input.\n");
                break;
        }
    }
}
