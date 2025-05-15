#ifndef AUD_H
#define AUD_H

/**
 * @file aud.h
 * @brief File responsible for declaring headers of auditing functions common to all UHB Additions Modules.
 */

// Check for auditing daemon presence separately in aud.c and aud.h?

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