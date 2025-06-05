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
 * @brief Checks if the firewall daemon is running.
 * @returns True if the daemon is running, false otherwise.
 */
bool check_firewall_status();

void initialize_firewall(bool copy_from_backup);

/**
 * @brief Restarts the firewall daemon. Used when applying firewall configuration.
 * @returns True if sucessful, false otherwise.
 */
bool restart_logging_daemon();

bool apply_firewall_confgiuration();

void add_firewall_rule();

void reset_firewall_configuration();

void view_firewall_configuration();

void view_firewall_manual();

#endif // FWL_H