#ifndef FWL_H
#define FWL_H

/**
 * @file aud.h
 * @brief File responsible for declaring headers of firewall functions common to all UHB Additions Modules.
 */

/**
 * @brief Checks if a firewall is present in the system.
 * @return True if detected, false otherwise.
 */
bool fwl_exists();

/**
 * 
 */
void check_ports();


#endif // FWL_H