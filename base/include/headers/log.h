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
bool check_log_status();

/**
 * @brief Show current rsyslog configuration in a more readable format.
 */
void check_log_config();

/**
 * @brief Set rsyslog to emit logs using the RFC3164/RFC5424 standard
 * @param state If this parameter is true, it will emit using RFC5424. 
 * Otherwise, it will maintain the default RFC3164 emission.
 */
void emit_rfc5424(bool state);

// Function for log size management

// Function for adding log levels categories

// Function to configure remote logging

// Function to allow

#endif // LOG_H