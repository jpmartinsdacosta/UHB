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
bool log_exists();
 
/**
 * @brief Checks if the logging daemon is running.
 * @returns True if the daemon is running, false otherwise.
 */
bool check_logging_status();

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
void initialize_logging();

// Configure log rotation settings

// Configure log levels

/**
 * @brief Auxiliary function that uncomments the UDP listening module.
 */
void enable_udp_module();

/**
 * @brief Auxiliary function that uncomments the TCP listening module.
 */
void enable_tcp_module();

/**
 * @brief Auxiliary function that finds, uncomments and changes the UDP module in rsyslog.
 * @param port The port to be used, in string format.
 * @returns True if successful, false otherwise.
 */
bool edit_udp_module(const char *port);

/**
 * @brief Auxiliary function that finds, uncomments and changes the TCP module in rsyslog.
 * @param port The port to be used, in string format.
 * @returns True if successful, false otherwise.
 */
bool edit_tcp_module(const char *port);

// Configure local log generation
void set_local_logging();


// Configure log transmission to a log server
void set_remote_logging();

// Configure local log server to receive logs from others
void set_log_server();

#endif // LOG_H