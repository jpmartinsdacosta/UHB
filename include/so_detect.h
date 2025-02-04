#ifndef SO_DETECT_H
#define SO_DETECT_H

/**
 * @brief Detect the operating system using uname -s.
 * 
 * This function attempts to detect the operating system by executing the 
 * 'uname -s' command. It returns an integer representing the detected OS.
 * 
 * @return int -1 if the OS is unknown, 0 if FreeBSD, 1 if Linux.
 */

int so_detect();

#endif // SO_DETECT_H