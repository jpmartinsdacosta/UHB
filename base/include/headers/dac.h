#include <stdbool.h>

#ifndef DAC_H
#define DAC_H

/**
 * @file dac.h
 * @brief Functions specific for Discretionary Access Control (DAC)
 * @note These functions are common to all linux/BSD operating systems.
 */

/**
 * @brief Get the DAC of a file.
 * This function gets the DAC of a file by executing "ls -l --" on the file.
 * @return true if the file exists, false otherwise.
 */
void get_dac();

/**
 * @brief Set the DAC of a file.
 * This function sets the DAC of a file by executing "chmod" and "chown" on the file.
 * It also adds the commands to the configuration file.
 */
void set_dac();

void rem_dac_entry();

void view_dac_configuration();

void reset_dac_configuration();

void apply_dac_configuration();

#endif // DAC_H