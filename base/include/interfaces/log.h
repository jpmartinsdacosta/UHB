#ifndef LOG_H
#define LOG_H

/**
 * @file log.h
 * @brief This file provides headers for functions regarding logging using rsyslog.
 */

/**
 * @brief Checks if a logging daemon is present in the system.
 * @return True if detected, false otherwise.
 */
bool rsyslog_exists();
 
/**
 * @brief Checks if the logging daemon is running.
 * @returns True if the daemon is running, false otherwise.
 */
bool check_rsyslog_status();

/**
 * @brief Detects the current RFC standard being used by rsyslog.
 * Changes the static boolean rfc_5424 in log.c to true or false
 * depending if the RFC5424 standard is being used.
 */

void detect_rfc5424();

/**
 * @brief Initializes rsyslog for UHB. If rsyslog is detected and enabled,
 * UHB changes the the static boolean rfc_5424 in log.c and copies the original
 * rsyslog.conf file from the default directory to the templates folder.
 */
void initialize_rsyslog();

// Configure log rotation settings

// Configure log levels

// Configure local log generation
bool set_local_logging();

// Configure log transmission
int set_remote_logging();


#endif // LOG_H