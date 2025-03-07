#ifndef IMP_BSD_H
#define IMP_BSD_H

/**
 *  @file imp_bsd.h
 *  @brief The BSD implementation.
 * 
 *  This function is called when the operating system is detected as FreeBSD.
 *  It provides UHB functions that are specific to the FreeBSD operating system.
 * 
 */

int imp_bsd(); // Test dummy function that returns 0.

/**
 * @brief Check if supported executables exist on the FreeBSD OS.
 * @param option The array to store the results of the check.
 * 
 */

void exec_exists_bsd(int option[4]);

/**
 * @brief Check if the user and group exist on the FreeBSD OS.
 * @param target The user or group to check.
 * @return True if the user or group exists, false otherwise.
 */

bool check_ug_bsd(char *target);

#endif // IMP_BSD_H