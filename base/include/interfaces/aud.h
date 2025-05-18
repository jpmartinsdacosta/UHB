#ifndef AUD_H
#define AUD_H

/**
 * @file aud.h
 * @brief File responsible for declaring headers of auditing functions common to all UHB Additions Modules.
 */

/**
 * @brief Checks if an auditing daemon is present in the system.
 * @return True if detected, false otherwise.
 */
bool aud_exists();

/**
 * @brief Checks whether the auditing daemon is running.
 * @returns True if the daemon is running, false otherwise.
 */
bool check_aud_status();

/**
 * @brief Show current auditd configuration in a more readable format.
 */
void check_aud_config();

// ...

#endif // AUD_H