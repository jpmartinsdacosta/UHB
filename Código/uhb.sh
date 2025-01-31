#!/bin/sh

# Global variable that determines the operating system and the commands that will be used in the script.
CMD_UHB

# Global variable that determines the user who is running the script.
UHB_USER

# Detect the current operating system.
OS="`uname`"

if $OS = "FreeBSD" then
    # Change a global variable so that all commands are compatible with BSD.
    CMD_UHB="bsd"
elif $OS = "Debian" then
    # Change a global variable so that all commands are compatible with Debian.
    CMD_UHB="debian"
else
    echo "Your operating system is not supported by UHB."
    exit 1
fi

# Check if the user is root or the uhb_user, otherwise exit the script.
if [ "$(id -u)" -eq 0 ] then
    UHB_USER="root"
elif [ "$(id -un)" = "uhb_user" ] then
    UHB_USER="uhb_user"
else
    echo "You do not have sufficient permissions to run UHB."
    exit 1
fi

# If the the operating system and user is root, then provide a menu with the available options.
if [ "$UHB   _USER" = "root" ] then
    echo "Welcome to UHB. Please select an option:"
    echo "1. List current DAC and ACL policies."
    echo "2. Modifiy current DAC and ACL policies."
    echo "3. Generate configuration file."
    echo "4. Exit UHB."
    read -p "Option: " OPTION
    case $OPTION in
        1)
            echo "Installing the UHB package..."
            ;;
        2)
            echo "Removing the UHB package..."
            ;;
        3)
            echo "Updating the UHB package..."
            ;;
        4)
            echo "Exiting UHB..."
            exit 0
            ;;
        *)
            echo "Invalid option. Exiting UHB..."
            exit 1
            ;;
    esac
fi
