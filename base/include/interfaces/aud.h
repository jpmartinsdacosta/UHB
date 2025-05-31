#ifndef AUD_H
#define AUD_H

// Config files to be edited.
#define UHB_AUDIT_CONTROL "/root/uhb/base/config/templates/audit_control"
#define UHB_AUDIT_USER "/root/uhb/base/config/templates/audit_user"
#define UHB_AUDIT_WARN "/root/uhb/base/config/templates/audit_warn"

// Backup original config files.
#define BACKUP_AUDIT_CONTROL "/root/uhb/base/config/templates/audit_control"
#define BACKUP_AUDIT_USER "/root/uhb/base/config/templates/audit_user"
#define BACKUP_AUDIT_WARN "/root/uhb/base/config/templates/audit_warn"

/**
 * @file aud.h
 * @brief File responsible for declaring headers of auditing functions common to all UHB Additions Modules.
 */

/**
 * Functions regarding daemon detection, status and execution.
 */

/**
 * @brief Checks if an auditing daemon is present in the system.
 * @return True if detected, false otherwise.
 */
bool aud_exists();

/**
 * @brief Checks if the separate service/daemon needed for remote auditing services is present
 * or not in the system.
 * @return True if detected, false otherwise.
 */
bool remote_auditing_daemon_exists();

/**
 * @brief Checks whether the auditing daemon is running.
 * @returns True if the daemon is running, false otherwise.
 */
bool check_auditing_status();

/**
 * @brief Restarts the auditing daemon.
 * @return True if successful, false otherwise.
 */
bool restart_auditing_daemon();

/**
 * Functions regarding module configuration
 */

/**
 * @brief Initializes the auditing module. This is done by copying the original config file(s)
 * to the uhb config folder to be modified by the user. Once modified, these are copied back
 * to the services configuration folder and its respective daemon is reset.
 * 
 * This function also copies the original configuration to the backup folder. If needed, instead
 * of copying from the original configuration file to UHB, it can be done from backup via the
 * boolean parameter in this function.
 * 
 * It will initialize the remote auditing daemon's configuration if present.
 * @param copy_from_backup          Boolean to determine whether to copy from backup.
 */
void initialize_auditing_module(bool copy_from_backup);

/**
 * @brief Resets the auditing module. This is done ONLY by copying the original audit service
 * configuration files located in the uhb/base/config/backups folder to the default auditing 
 * directory.
 * 
 * The existence/availability of backup configuration files is of the responsibility of the
 * user and the correct use of the initailize_auditing function when copying from backup or
 * not.
 */
void reset_auditing_configuration();

/**
 * @brief View current auditing configuration. This function exists for audit services which
 * contain more than one configuration file.
 */
void view_auditing_configuration();

/**
 * Functions regarding the addition of custom auditing policy
 */

/**
 * @brief Add auditing configuration.
 * @return True if successful, false otherwise.
 */
void add_local_auditing();

/**
 * Functions regarding the reception and forwarding of audit messages to the network.
 */

/**
 * @brief Configures the reception of audit messages from remote systems.
 */
void configure_auditing_reception_service();

/**
 * @brief Configures the forwarding of audit messages to remote systems.
 */
void configure_auditing_forwarding_service();

#endif // AUD_H